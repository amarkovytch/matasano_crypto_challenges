#include "aes_ecb_oracle.h"
#include "byte_data.h"
#include "crypto_constants.h"
#include "matasano_asserts.h"
#include "padder.h"

#include <functional>

std::optional<std::size_t> AesEcbOracle::detectEqualBlockNumAfterEncryption(ByteData &plain, std::size_t blockSize)
{
    auto encrypted = encryptor_(plain);

    auto blocks = encrypted.extractRows(blockSize);
    for (std::size_t i = 0; i < blocks.size() - 1; i++)
    {
        if (blocks.at(i) == blocks.at(i + 1))
        {
            return i;
        }
    }

    return {};
}

std::optional<std::size_t> AesEcbOracle::guessBlockSize()
{
    for (std::size_t guessSize = 8; guessSize <= CryptoConstants::BLOCK_SIZE_BYTES * 2; guessSize++)
    {
        ByteData plain(0, std::size_t{guessSize * 3});
        auto equalBlockNum = detectEqualBlockNumAfterEncryption(plain, guessSize);
        if (equalBlockNum)
        {
            LOGIC_ASSERT(guessSize == CryptoConstants::BLOCK_SIZE_BYTES);
            return guessSize;
        }
    }

    return {};
}

bool AesEcbOracle::isEcb()
{
    ByteData plain(0, std::size_t{CryptoConstants::BLOCK_SIZE_BYTES * 3});
    return detectEqualBlockNumAfterEncryption(plain, CryptoConstants::BLOCK_SIZE_BYTES).has_value();
}

void AesEcbOracle::validatePlainDataOffsetStruct(const AesEcbOracle::OffsetToAddToLookForSecret &offset)
{
    THROW_IF(offset.inBytes >= CryptoConstants::BLOCK_SIZE_BYTES,
             "the offset in bytes should be less than a block size", std::invalid_argument);

    switch (encryptorType_)
    {
    case (EncryptorType::ConstantRandom_Plain_Secret):
        break;
    case (EncryptorType::Plain_Secret):
        THROW_IF(offset.inBytes != 0 || offset.inBlocks != 0, "For Plain_Secret boths offsets should be 0",
                 std::invalid_argument);
        break;
    default:
        LOGIC_SHOULD_NOT_REACH_THAT_POINT();
    }
}

AesEcbOracle::OffsetToAddToLookForSecret AesEcbOracle::guessPlainOffset()
{
    auto blockSize = guessBlockSize();
    LOGIC_ASSERT(blockSize);

    ByteData plain(0, std::size_t{*blockSize * 2});
    for (std::size_t numOfAdditionalBytes = 0; numOfAdditionalBytes < *blockSize; numOfAdditionalBytes++)
    {
        auto equalBlockNum = detectEqualBlockNumAfterEncryption(plain, *blockSize);
        if (equalBlockNum)
        {
            AesEcbOracle::OffsetToAddToLookForSecret res{*equalBlockNum, numOfAdditionalBytes};
            validatePlainDataOffsetStruct(res);

            return res;
        }

        plain += 0;
    }

    LOGIC_SHOULD_NOT_REACH_THAT_POINT();
}

ByteData AesEcbOracle::recoverSecret()
{
    THROW_IF(encryptorType_ == EncryptorType::RandomUpToBlock1_Plain_RandomUpToBlock2,
             "RandomUpToBlock1_Plain_RandomUpToBlock2 is not supported for recovering secret", std::invalid_argument);

    auto offset = guessPlainOffset();

    auto encryptedJustBytesOffset = encryptor_(ByteData(0, std::size_t{offset.inBytes}));
    LOGIC_ASSERT(encryptedJustBytesOffset.size() % CryptoConstants::BLOCK_SIZE_BYTES == 0);

    auto numOfBlocks = encryptedJustBytesOffset.size() / CryptoConstants::BLOCK_SIZE_BYTES;

    ByteData res;

    ByteData prevBlock(0, std::size_t{CryptoConstants::BLOCK_SIZE_BYTES});
    for (std::size_t blockNum = 0; blockNum < numOfBlocks - offset.inBlocks; blockNum++)
    {
        auto decipheredBlock = recoverSecretBlock(prevBlock, blockNum, offset);
        prevBlock = decipheredBlock;
        res += decipheredBlock;
    }

    return res;
}

ByteData AesEcbOracle::recoverSecretBlock(const ByteData &prevBlockPlain, std::size_t blockNum,
                                          const AesEcbOracle::OffsetToAddToLookForSecret &offset)
{
    ByteData partOfNextBlockGuessedSoFar;
    std::size_t uknownPartSize = CryptoConstants::BLOCK_SIZE_BYTES;
    std::size_t i = 1;
    do
    {
        auto curBlockWithoutFirstNBytes = prevBlockPlain.subData(i, uknownPartSize - 1);
        auto guessedByte =
            guessNthByteInNextBlock(curBlockWithoutFirstNBytes, partOfNextBlockGuessedSoFar, blockNum, offset);
        if (!guessedByte)
        {
            // if we encountered padding, it means that the last guessed byte was also redundant
            partOfNextBlockGuessedSoFar.popBack();
            break;
        }
        partOfNextBlockGuessedSoFar += *guessedByte;
        uknownPartSize--;
        i++;
    } while (uknownPartSize != 0);

    return partOfNextBlockGuessedSoFar;
}

std::optional<std::uint8_t>
AesEcbOracle::guessNthByteInNextBlock(const ByteData &curBlockWithoutFirstNBytes,
                                      const ByteData &partOfNextBlockGuessedSoFar, size_t blockNum,
                                      const AesEcbOracle::OffsetToAddToLookForSecret &offset)
{
    LOGIC_ASSERT(curBlockWithoutFirstNBytes.size() + partOfNextBlockGuessedSoFar.size() ==
                 CryptoConstants::BLOCK_SIZE_BYTES - 1);

    ByteData offsetBytesPrepend(0, std::size_t{offset.inBytes});

    auto encryptedFirstNBytesOfSecret = encryptor_(offsetBytesPrepend + curBlockWithoutFirstNBytes)
                                            .extractRow(CryptoConstants::BLOCK_SIZE_BYTES, blockNum + offset.inBlocks);
    LOGIC_ASSERT(encryptedFirstNBytesOfSecret.size() != 0);

    std::uint8_t curByte = 0;
    auto curBlockWithoutLastByte = curBlockWithoutFirstNBytes + partOfNextBlockGuessedSoFar;

    do
    {
        auto bytePermutation = encryptor_(offsetBytesPrepend + curBlockWithoutLastByte + curByte)
                                   .extractRow(CryptoConstants::BLOCK_SIZE_BYTES, offset.inBlocks);
        LOGIC_ASSERT(bytePermutation.size() != 0);

        if (encryptedFirstNBytesOfSecret == bytePermutation)
        {
            return curByte;
        }
    } while (++curByte != 0);

    // if we reached this point and were not able to guess, it means we reached the padding part which changes
    // depending the number of the bytes missing till the end of the block, we need to stop
    return {};
}