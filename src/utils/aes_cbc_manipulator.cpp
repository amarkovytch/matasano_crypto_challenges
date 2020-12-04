#include "aes_cbc_manipulator.h"
#include "crypto_constants.h"
#include "general_utils.h"
#include "padder.h"

ByteData AesCbcManipulator::manipulateNextBlockAfterPlain(const ByteData &desiredPostPlain)
{
    auto blockSize = CryptoConstants::BLOCK_SIZE_BYTES;

    THROW_IF(desiredPostPlain.size() > CryptoConstants::BLOCK_SIZE_BYTES,
             "the desiredPostPlain size should be up to block...", std::invalid_argument);

    auto preSize = encryptor_.pre().size();
    auto preBlocks = GeneralUtils::ceil(preSize, CryptoConstants::BLOCK_SIZE_BYTES);
    auto preSizeOfLastBlock = preSize % CryptoConstants::BLOCK_SIZE_BYTES;
    auto sizeToAddToStartAtNewBlock =
        (CryptoConstants::BLOCK_SIZE_BYTES - preSizeOfLastBlock) % CryptoConstants::BLOCK_SIZE_BYTES;

    ByteData dataToMakePlainStartAtNewBlock(0, std::size_t(sizeToAddToStartAtNewBlock));

    auto pre_plan_firstBlockOfPost_Encrypted =
        encryptor_.encrypt(dataToMakePlainStartAtNewBlock + ByteData(0, std::size_t{blockSize}))
            .extractRows(blockSize, preBlocks + 2);

    // just in case we have full additional block of padding, we want to discard it
    auto desiredPostPadded = Padder::padToBlockSize(desiredPostPlain).extractRow(blockSize, 0);

    // take only the first block of post
    auto postPadded = Padder::padToBlockSize(encryptor_.post().extractRow(blockSize, 0)).extractRow(blockSize, 0);

    // during decryption after xoring with previous original ciphertext we will get original postPadded xored with
    // desiredPostPadded. The postPadded should cancel out
    pre_plan_firstBlockOfPost_Encrypted[preBlocks] ^= desiredPostPadded ^ postPadded;

    return pre_plan_firstBlockOfPost_Encrypted;
}