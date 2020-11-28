#ifndef MATASANO_PADDER_H
#define MATASANO_PADDER_H

#include "byte_data.h"
#include <cstdint>

/**
 * @brief Provides various padding services
 */
class Padder
{
public:
    /**
     * @brief pads a given ByteData with a given number of bytes in PKCS#7 format
     * The value of all padded bytes equal to the total number to be padded :
     * bytesNumToPad = 4 : \x04\x04\x04\x04
     *
     * @param b ByteDada to pad
     * @param bytesNumToPad the number of bytes to pad, should be less than 256
     */
    static ByteData pad(const ByteData &data, std::uint8_t bytesNumToPad);

    /**
     * @brief Add missing padding bytes to block which size is less or equal to blockSize
     * The padding is done accoring to PKCS#7 format :
     * 1. If for example bytes are missing then this will be added : \x04\x04\x04\x04
     * 2. If size of block is blockSize then an addinional full block of padding will be added
     *
     * @param block with size less or equal to blockSize
     * @param unpaddedBlock the block size
     * @return padded ByteData
     *
     * @throw std::invalid_argument if block size > blockSize
     */
    static ByteData padToBlockSize(const ByteData &unpaddedBlock, std::uint8_t blockSize);

    /**
     * @brief Removes padding that was added according to PKCS#7 format (@see padToBlockSize)
     *
     * @param block padded block
     * @return ByteData block without padding
     *
     * @throw std::invalid_argument if padding is invalid
     */
    static ByteData removePadding(const ByteData &paddedBlock);

private:
    /**
     * @brief Make sure the padding is according to pkcs#7 format (@see padToBlockSize)
     * If it is not - throw exception
     *
     * @param paddedBlock the block that has padding
     * @param bytesNumToPad the length of the padding
     *
     * @throw std::invalid_argument if the padding is invalid
     */
    static void validatePadding(const ByteData &paddedBlock, std::uint8_t bytesNumToPad);
};

#endif
