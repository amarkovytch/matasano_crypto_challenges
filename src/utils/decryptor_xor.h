#ifndef MATASANO_DECRYPTOR_H
#define MATASANO_DECRYPTOR_H

#include "byte_data.h"
#include "byte_distribution.h"
#include <cstddef>
#include <utility>

/**
 * A decryptor for various xor ciphers
 * Currently only supports single-byte xor
 */
class DecryptorXor
{
public:
    /**
     * @brief Construct a new Decryptor Xor object. You need to provide ByteData objectwith long enough text in the
     * language you believe your ciphered message is
     *
     * @param referenceLanguageData reference string in the language you believe your ciphered message is. Should be
     * long enough to make statistical analysis possible, the longer it is the more accurate analysis is going to be
     */
    DecryptorXor(const std::string &referenceLanguageData);

    /**
     * @brief Try to decipher given text. Return the deciphered string, one byte key and measure of confidence. The
     * deciphering is done by applying (xoring) all the possible key variants one by one and comparing the resulting
     * characters distribution to the distribution of the reference data. The result that has the closest match will be
     * selected as deciphered text. The measure of confidence is a floating point number, the smaller it is - the higher
     * the confidence (can be used when comparing the results of deciphering different ciphered data)
     *
     * @param cipheredText - ciphered text
     * @return deciphered text, one-byte xor key, measure of confidence
     */
    std::tuple<std::string, std::byte, double> decipher(const ByteData &cipheredData) const;

private:
    ByteDistribution referenceLanguage;
};

#endif