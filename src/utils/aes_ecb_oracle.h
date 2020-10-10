#ifndef MATASANO_AES_ECB_ORACLE_H
#define MATASANO_AES_ECB_ORACLE_H

#include <functional>

#include "byte_data.h"

class AesEcbOracle
{
public:
    /**
     * @brief encryptorFunction type
     * Receives plain ByteData, returns encrypted ByteData
     */
    using ecryptorFunction = std::function<ByteData(const ByteData &)>;
    // typedef ByteData (*ecryptorFunction)(const ByteData &);

    /**
     * @brief Checks whether the given encrypting function is operating in Aes Ecb mode
     * The oracle can tolerate of up to block of data prepended internally by the encryptor to the plain data it
     * receives and any number of bytes appended to it. Otherwise the prediciton will be wrong.
     *
     * @param encryptor encrypting function
     * @return true if encryptor is ecb, false otherwise
     */
    static bool isEcb(ecryptorFunction encryptor);
};

#endif
