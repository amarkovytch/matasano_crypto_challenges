#ifndef MATASANO_AES_CBC_MANIPULATOR_H
#define MATASANO_AES_CBC_MANIPULATOR_H

#include "aes.h"
#include "byte_data.h"
#include "general_utils.h"
#include <string>

/**
 * @brief Allows tricking AesCbc encryptor into producing an encrypted result such that after decryption we will get a
 * desired plaintext (without knowing the key)
 */
class AesCbcManipulator
{
public:
    /**
     * @brief Aes Cbc encryptor that prepends and appends known and constant data to the given plain
     */
    class EncryptorConstPrePlainConstPost
    {
    public:
        EncryptorConstPrePlainConstPost(const ByteData &pre, const ByteData &post, const ByteData &key,
                                        const ByteData &iv)
            : pre_(pre), post_(post), aes_(key, iv){};

        inline const ByteData &post() const { return post_; }
        inline const ByteData &pre() const { return pre_; }

        /**
         * @brief Encrypt given plain data in Aes Cbc mode with constant key and random data
         * The plain is prepended with pre and appended with post (which were supplied during construction)
         *
         * @param plain data to encrypt
         * @return encrypted data
         */
        inline ByteData encrypt(const ByteData &plain) const { return aes_.encrypt(pre_ + plain + post_); }

    private:
        ByteData pre_;
        ByteData post_;
        Aes aes_;
    };

    AesCbcManipulator(const AesCbcManipulator::EncryptorConstPrePlainConstPost &encryptor) : encryptor_(encryptor){};

    /**
     * @brief Manipulate the encryptor such that the result of encryption will have the desired result block after the
     * controlled (plain) block
     * @note plain will be garbage after dercryption (the assumption is that the plain is not important to preserve)
     *
     * @param desiredBlock to get after the controlled plain. Should be up to (and including) block size
     *
     * @return the manipulated encrypted result (which will give what specified above after decryption)
     * @throw std::invalid_argument if desiredPostPlain size is more than block
     */
    ByteData manipulateNextBlockAfterPlain(const ByteData &desiredPostPlain);

private:
    EncryptorConstPrePlainConstPost encryptor_;
};

#endif