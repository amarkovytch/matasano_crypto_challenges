#ifndef MATASANO_AES_H
#define MATASANO_AES_H

#include "byte_data.h"
#include <coroutine>
#include <vector>

/**
 * @brief Aes encryption services
 */
class Aes
{
public:
    /**
     * @brief encryption mode
     */
    enum class Mode
    {
        ecb, // ECB mode
        cbc  // CBC mode
    };

    /**
     * @brief key size
     *
     */
    enum class KeySize
    {
        bit128, // 16 byte key size
    };

    /**
     * @brief Construct a new Aes object
     *
     * @param key Key to use for encryption / decryption
     * @param iv IV , empty by default
     * @param mode encryption / decryption mode
     * @param keySize encryption / decryption key size
     *
     * @throw std::invalid_argument on invalid mode or KeySize or invalid iv for the mode where it is requred or if
     * KeySize is not equal to provided key size
     */
    Aes(const ByteData &key, const ByteData &iv, Mode mode = Mode::cbc, KeySize keySize = KeySize::bit128);

    /**
     * @brief Encrypts the given plain data
     *
     * @param plain plain data
     * @return resulting encrypted data
     *
     * @throw std::invalid_argument if plain is empty
     */
    ByteData encrypt(const ByteData &plain) const;

    /**
     * @brief Decrypts the gived ciphered data
     *
     * @param cipher ciphered data
     * @return plain data
     *
     * @throw std::invalid_argument if cipher is empty
     */
    ByteData decrypt(const ByteData &cipher) const;

private:
    /**
     * @brief encryption key
     */
    ByteData key_;

    /**
     * @brief iv
     */
    ByteData iv_;

    /**
     * @brief mode of operation
     */
    Mode mode_;

    /**
     * @brief key size
     */
    KeySize keySize_;

    /**
     * @brief Encrypts / decrypts the given secret plain data
     *
     * @param data data to encrypt / decrypt
     * @param encrypt if true encrypt, otherwise decrypt
     * @return resulting encrypted data
     *
     * @throw std::invalid_argument if plain is empty
     */
    ByteData encryptDecrypt(const ByteData &data, bool encrypt) const;

    /**
     * @brief Encrypts / Decrypts a single data that is multiple of blocks size
     *
     * @param block data that is multiple of block size
     * @param encrypt if true - encrypt, otherwise decrypt
     * @return the encrypted / decrypted data
     */
    ByteData ecbEncryptDecryptBlock(const ByteData &block, bool encrypt) const;

    /**
     * @brief Perform ecb encryption / decryption on a given data
     *
     * @param data data to encrypt / decrypt
     * @param encrypt if true - encrypt, otherwise decrypt
     * @return encrypted data
     */
    ByteData encryptDecryptEcb(const ByteData &data, bool encrypt) const;

    /**
     * @brief Perform cbc encryption / decryption on a given data
     *
     * @param data data to encrypt / decrypt
     * @param encrypt if true - encrypt, otherwise decrypt
     * @return encrypted data
     */
    ByteData encryptDecryptCbc(const ByteData &data, bool encrypt) const;
};

#endif