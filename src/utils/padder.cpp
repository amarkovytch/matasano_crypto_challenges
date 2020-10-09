#include <span>

#include "byte_data.h"
#include "matasano_asserts.h"
#include "padder.h"

ByteData Padder::pad(const ByteData &data, std::uint8_t bytesNumToPad)
{
    std::vector pad(bytesNumToPad, bytesNumToPad);
    return data + pad;
}

ByteData Padder::padToBlockSize(const ByteData &unpaddedBlock, std::uint8_t blockSize)
{
    THROW_IF(unpaddedBlock.size() > blockSize,
             "the size of the block " + std::to_string(unpaddedBlock.size()) +
                 " is bigger than size of the blocksize " + std::to_string(blockSize),
             std::invalid_argument);

    std::uint8_t bytesNumToPad = blockSize - unpaddedBlock.size();
    if (0 == bytesNumToPad)
    {
        bytesNumToPad = blockSize;
    }

    return pad(unpaddedBlock, bytesNumToPad);
}

ByteData Padder::removePadding(const ByteData &paddedBlock)
{
    if (paddedBlock.size() == 0)
    {
        return paddedBlock;
    }

    std::uint8_t bytesToRemove = paddedBlock.secureData().back();

    THROW_IF(bytesToRemove > paddedBlock.size(),
             "the padding is wrong, there are " + std::to_string(bytesToRemove) +
                 " bytes to be removed, which is more than the total size of the block:" +
                 std::to_string(paddedBlock.size()),
             std::invalid_argument);

    validatePadding(paddedBlock, bytesToRemove);

    return paddedBlock.subData(0, paddedBlock.size() - bytesToRemove);
}

void Padder::validatePadding(const ByteData &paddedBlock, std::uint8_t padByte)
{
    for (std::size_t i = paddedBlock.size() - padByte; i < paddedBlock.size(); i++)
    {
        THROW_IF(paddedBlock.secureData().at(i) != padByte, "the padding is wrong", std::invalid_argument);
    }
}