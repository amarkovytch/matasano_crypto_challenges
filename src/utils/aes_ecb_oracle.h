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
    enum class EncryptorType
    {
        /**
         * @brief some random data with size up to block is appended before plain, some other random data with size up
         * to block is appended after plain
         */
        RandomUpToBlock1_Plain_RandomUpToBlock2,

        /**
         * @brief constant random data (of any size) is appended before the plan, some other secret data (any size)
         * appears after the plain
         */
        ConstantRandom_Plain_Secret,

        /**
         * @brief secret data (of any size) is added after plain
         */
        Plain_Secret
    };

    /**
     * @brief encryptorFunction type
     * Receives plain ByteData, returns encrypted ByteData according to the EcryptorType
     */
    using encryptorFunction = std::function<ByteData(const ByteData &plain)>;

    AesEcbOracle(encryptorFunction encryptor, EncryptorType encryptorType)
        : encryptor_(encryptor), encryptorType_(encryptorType)
    {
        switch (encryptorType)
        {
        case (EncryptorType::RandomUpToBlock1_Plain_RandomUpToBlock2):
            break;
        case (EncryptorType::ConstantRandom_Plain_Secret):
            break;
        case (EncryptorType::Plain_Secret):
            break;
        default:
            throw std::invalid_argument("Unsupported encryptor type");
        }
    };

    /**
     * @brief Checks whether the given encrypting function is operating in Aes Ecb mode
     * Any EcryptorType is supported
     *
     * @return true if encryptorAppendPrependToPlain is ecb, false otherwise
     */
    bool isEcb();

    /**
     * @brief recovers the secret text from the encryptor function
     * Those are the encryptor types that are supported:
     * Random_Plain_Secret
     * Plain_Secret
     *
     * @return uncovered plain data
     * @throw std::invalid_argument if 'EcryptorType' is not of the supported type
     */
    ByteData recoverSecret();

private:
    encryptorFunction encryptor_;
    EncryptorType encryptorType_;

    /**
     * @brief Represents an offset that should be added each time when looking for secret data (in blocks and bytes)
     * @note this is not the same as the actual offset of secret data
     */
    struct OffsetToAddToLookForSecret
    {
        std::size_t inBlocks;
        std::size_t inBytes;
    };

    /**
     * @brief validate the PlainDataOffset according to the type of encryptor function
     * For example, for Random_Plain_Secret encryptor type if random data has the size of 47 bytes, the structure will
     * look like this: blocksOffset = 2, bytesOffset = 15
     *
     * @param offset the PlainDataOffset struct
     * @throw std::invalid_argument if there is a mismatch
     */
    void validatePlainDataOffsetStruct(const AesEcbOracle::OffsetToAddToLookForSecret &offset);

    /**
     * @brief Guess the offset of the plain data in the encryptor function
     *
     * @return PlainDataOffset structure representing the offset
     * @throw std::invalid_argument if encryptor type does not correspond to the results. For example for
     * SecretUpToBlock1_Plain_SecretUpToBlock2 the number of blocks should be 0 and the number of bytes should be up
     * to block size
     */
    AesEcbOracle::OffsetToAddToLookForSecret guessPlainOffset();

    /**
     * @brief use encryptor function on a given plain data, detect repeaping subsequent blocks (if any) and return the
     * number of the first block
     *
     * @param plain the plain data
     * @param blockSize the block size
     * @return std::optional<std::size_t> if 2 repeaping subsequent blocks are detected, return the number of the first
     * one
     */
    std::optional<std::size_t> detectEqualBlockNumAfterEncryption(ByteData &plain, std::size_t blockSize);

    /**
     * @brief Guesses the block size of the encryptorFunction
     *
     * @return the block size or none if not found
     * @throw std::invalid_argument if function can't guess the block size for the given encryptor funciton
     */
    std::optional<std::size_t> guessBlockSize();

    /**
     * @brief recovers one block of the secret text from the encryptor function
     *
     * @param prevBlockPlain previous block of data recovered (or just any block of data for the 0-th block)
     * @param blockNum the block number to recover
     * @param offset the offset of the plain data
     *
     * @return recovered block of data
     */
    ByteData recoverSecretBlock(const ByteData &prevBlockRecovered, std::size_t blockNum,
                                const AesEcbOracle::OffsetToAddToLookForSecret &offset);

    /**
     * @brief Receives block without last n bytes, part of the next block guessed so far with length n - 1 and guesses
     * n-th byte in the original secret text from encryptor function
     *
     * @param curBlockWithoutFirstNBytes block of data without first n bytes
     * @param partOfNextBlockGuessedSoFar part of the next block guessed so far (assumed to be n - 1 length)
     * @param blockNum the number of block we are trying to guess. Should be within the number of blocks in secret data
     * @param offset the offset of the plain data
     *
     * @return Nth byte in the original secret text or nothing (this means we reached padding and previous guessed byte
     * is also irrelevant)
     */
    std::optional<std::uint8_t> guessNthByteInNextBlock(const ByteData &curBlockWithoutFirstNBytes,
                                                        const ByteData &partOfNextBlockGuessedSoFar, size_t blockNum,
                                                        const AesEcbOracle::OffsetToAddToLookForSecret &offset);
};

#endif
