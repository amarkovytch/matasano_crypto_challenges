#include "aes_ecb_oracle.h"
#include "byte_data.h"
#include "crypto_constants.h"
#include "matasano_asserts.h"
#include "padder.h"

#include <functional>

bool AesEcbOracle::isEcb()
{
    ByteData identicalData3blocks(std::vector(CryptoConstants::BLOCK_SIZE_BYTES * 3, std::uint8_t{1}));
    auto result = encryptor_(identicalData3blocks);

    // if this is ecb - the encrypted second and third blocks should be identical
    auto blocks = result.extractRows(CryptoConstants::BLOCK_SIZE_BYTES);

    return blocks.at(1) == blocks.at(2);
}

ByteData AesEcbOracle::recoverSecret()
{
    THROW_IF(encryptorType_ != AesEcbOracle::EcryptorType::Plain_Secret,
             "this function is supported only for 'Plain_Secret' encryptor type", std::invalid_argument);
    auto blockSize = guessBlockSize();
    LOGIC_ASSERT(isEcb());
    LOGIC_ASSERT(blockSize == CryptoConstants::BLOCK_SIZE_BYTES);

    auto encryptedPlain = encryptor_(ByteData());
    LOGIC_ASSERT(encryptedPlain.size() % blockSize == 0);

    auto numOfBlocks = encryptedPlain.size() / blockSize;

    ByteData res;
    ByteData prevBlock(blockSize);
    for (std::size_t blockNum = 0; blockNum < numOfBlocks; blockNum++)
    {
        auto decipheredBlock = recoverSecretBlock(prevBlock, blockNum);
        prevBlock = decipheredBlock;
        res += decipheredBlock;
    }

    return res;
}

std::size_t AesEcbOracle::guessBlockSize()
{
    for (std::size_t guessSize = 1; guessSize <= CryptoConstants::BLOCK_SIZE_BYTES * 2; guessSize++)
    {
        auto prepend = ByteData(std::vector(guessSize * 2, std::uint8_t{1}));
        auto result = encryptor_(prepend);

        auto blocks = result.extractRows(guessSize);
        if (blocks.at(0) == blocks.at(1))
        {
            return guessSize;
        }
    }

    THROW_IF(true, "wasn't able to guess block size with given encryptor function", std::invalid_argument);
}

ByteData AesEcbOracle::recoverSecretBlock(const ByteData &prevBlockPlain, std::size_t blockNum)
{
    ByteData partOfNextBlockGuessedSoFar;
    std::size_t uknownPartSize = CryptoConstants::BLOCK_SIZE_BYTES;
    std::size_t i = 1;
    do
    {
        auto curBlockWithoutFirstNBytes = prevBlockPlain.subData(i, uknownPartSize - 1);
        auto guessedByte = guessNthByteInNextBlock(curBlockWithoutFirstNBytes, partOfNextBlockGuessedSoFar, blockNum);
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

std::optional<std::uint8_t> AesEcbOracle::guessNthByteInNextBlock(const ByteData &curBlockWithoutFirstNBytes,
                                                                  const ByteData &partOfNextBlockGuessedSoFar,
                                                                  size_t blockNum)
{
    LOGIC_ASSERT(curBlockWithoutFirstNBytes.size() + partOfNextBlockGuessedSoFar.size() ==
                 CryptoConstants::BLOCK_SIZE_BYTES - 1);

    auto encryptedFirstByteOfSecret =
        encryptor_(curBlockWithoutFirstNBytes).extractRow(CryptoConstants::BLOCK_SIZE_BYTES, blockNum);
    LOGIC_ASSERT(encryptedFirstByteOfSecret.size() != 0);

    std::uint8_t curByte = 0;
    auto curBlockWithoutLastByte = curBlockWithoutFirstNBytes + partOfNextBlockGuessedSoFar;

    do
    {
        auto bytePermutation =
            encryptor_(curBlockWithoutLastByte + curByte).extractRow(CryptoConstants::BLOCK_SIZE_BYTES, 0);
        LOGIC_ASSERT(bytePermutation.size() != 0);

        if (encryptedFirstByteOfSecret == bytePermutation)
        {
            return curByte;
        }
    } while (++curByte != 0);

    // if we reached this point and were not able to guess, it means we reached the padding part which changes depending
    // the number of the bytes missing till the end of the block, we need to stop
    return {};
}