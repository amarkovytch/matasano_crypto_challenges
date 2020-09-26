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
    static ByteData pad(const ByteData &b, std::uint8_t bytesNumToPad);
};

#endif
