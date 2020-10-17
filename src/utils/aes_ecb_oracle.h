#ifndef MATASANO_AES_ECB_ORACLE_H
#define MATASANO_AES_ECB_ORACLE_H

#include <functional>
#include <map>

#include "byte_data.h"

/**
 * @brief various Ecb decryption services that receive some encryptor function that encrypts data with unknown secret
 * key. Some of the data received by encryptor can be controlled - 'plain', some is uknown 'secret' added internally by
 * encryptor before the encryption occurrs. Various services below allow various degrees of info to be discovered about
 * the secret data, the way encyrptor function works, etc.
 */
class AesEcbOracle
{
public:
    /**
     * @brief Type of encryptor function
     */
    enum class EcryptorType
    {
        /**
         * @brief some secret data with size up to block is appended before plain, some other secret data with size up
         * to block is appended after plain
         */
        SecretUpToBlock1_Plain_SecretUpToBlock2,

        /**
         * @brief secret data (of any size) is added after plain
         */
        Plain_Secret
    };

    /**
     * @brief encryptorFunction type
     * Receives plain ByteData, returns encrypted ByteData according to the EcryptorType
     */
    using ecryptorFunction = std::function<ByteData(const ByteData &plain)>;

    AesEcbOracle(ecryptorFunction encryptor, EcryptorType encryptorType)
        : encryptor_(encryptor), encryptorType_(encryptorType){};

    /**
     * @brief Checks whether the given encrypting function is operating in Aes Ecb mode
     * Any EcryptorType is supported
     *
     * @return true if encryptorAppendPrependToPlain is ecb, false otherwise
     */
    bool isEcb();

    /**
     * @brief recovers the secret text from the 'Plain_Secret' encryptor function
     *
     * @return uncovered plain data
     * @throw std::invalid_argument if 'EcryptorType' is not Plain_Secret
     */
    ByteData recoverSecret();

private:
    ecryptorFunction encryptor_;
    EcryptorType encryptorType_;

    /**
     * @brief Guesses the block size of the encryptorFunction
     *
     * @return the block size
     * @throw std::invalid_argument if function can't guess the block size for the given encryptor funciton
     */
    std::size_t guessBlockSize();

    /**
     * @brief recovers one block of the secret text from the 'Plain_Secret' encryptor function
     *
     * @param prevBlockPlain previous block of data recovered (or just any block of data for the 0-th block)
     * @param blockNum the block number to recover
     * @return recovered block of data
     */
    ByteData recoverSecretBlock(const ByteData &prevBlockRecovered, std::size_t blockNum);

    /**
     * @brief Receives block without last n bytes, part of the next block guessed so far with length n - 1 and guesses
     * n-th byte in the original secret text from 'Plain_Secret' encryptor function
     *
     * @param curBlockWithoutFirstNBytes block of data without first n bytes
     * @param partOfNextBlockGuessedSoFar part of the next block guessed so far (assumed to be n - 1 length)
     * @param blockNum the number of block we are trying to guess. Should be within the number of blocks in secret data
     * @return Nth byte in the original secret text or nothing (this means we reached padding and previous guessed byte
     * is also irrelevant)
     */
    std::optional<std::uint8_t> guessNthByteInNextBlock(const ByteData &curBlockWithoutFirstNBytes,
                                                        const ByteData &partOfNextBlockGuessedSoFar, size_t blockNum);
};

#endif
