#ifndef MATASANO_DECRYPTOR_H
#define MATASANO_DECRYPTOR_H

#include "byte_data.h"
#include "byte_distribution.h"
#include <cstddef>
#include <utility>

/**
 * A decryptor for various xor ciphers
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
     * @brief Try to decipher given text that was ciphered with single byte key. Return the deciphered string, one byte
     * key and measure of confidence. The deciphering is done by applying (xoring) all the possible key variants one by
     * one and comparing the resulting characters distribution to the distribution of the reference data. The result
     * that has the closest match will be selected as deciphered text. The measure of confidence is a floating point
     * number, the smaller it is - the higher the confidence (can be used when comparing the results of deciphering
     * different ciphered data)
     *
     * @param cipheredText - one byte key ciphered text, can't be empty
     * @return deciphered text, one-byte xor key, measure of confidence
     * @throw std::invalid_argument if cipheredText is empty
     */
    std::tuple<std::string, std::byte, double> decipherSingle(const ByteData &cipheredData) const;

    /**
     * @brief Try to decipher given text that was ciphered with multi byte key. Return the deciphered string, multi byte
     * key and measure of confidence. The deciphering is done by first detecting the key size, then guessing each byte
     * of the key separately, using 'decipherSingle' against corresponding part of the encrypted data that was xored
     * with this specific byte of the key. The measure of confidence is a floating point number, the smaller it is - the
     * higher the confidence (can be used when comparing the results of deciphering different ciphered data)
     *
     * @param cipheredText - one byte key ciphered text, can't be empty
     * @param keySizeRange - the range of possible key sizes to try. The minimum should be no less than 2. The maximum
     * should be bigger than minimum. Max key size should be small enough compared to cipheredData size, otherwise
     * chances of decrypting the text correctly - are low
     *
     * @return deciphered text, multi-byte xor key, measure of confidence
     * @throw std::invalid_argument if range is not according to what is defined above or if cipheredText is empty
     */
    std::tuple<std::string, ByteData, double>
    decipherMulti(const ByteData &cipheredData, const std::pair<std::size_t, std::size_t> &keySizeRange) const;

private:
    /**
     * reference language distribution. Used to 'guess' whether we deciphered the given byte data vector correctly
     */
    ByteDistribution referenceLanguage_;

    /**
     * @brief Tries to guess the key size with which the given ByteData was ciphered.The idea is to try various key
     * sizes consequently and see for which blocks do we have the least hamming distance. Since this is pretty
     * innacurate, a couple of best key sizes will be returned and tried out
     *
     * @param cipheredData - the ciphered object
     * @param keySizeRange - the range of possible key sizes to try. The minimum should be no less than 2. The
     * maximum should be bigger than minimum. Max key size should be small enough compared to cipheredData size,
     * otherwise chances of decrypting the text correctly - are low
     * @param numberOfCandidatesToReturn - the number of candidate key sizes to be in the resulting vector
     *
     * @return the vector of best 'numberOfCandidatesToReturn' key sizes
     * @throw std::invalid_argument if range is not according to what is defined above
     */
    std::vector<std::size_t> guessKeySize(const ByteData &cipheredData,
                                          const std::pair<std::size_t, std::size_t> &keySizeRange,
                                          std::size_t numberOfCandidatesToReturn = 3) const;

    /**
     * @brief Measures level of confidence whether a given deciphered data resembles a referenced language
     * The confidence is actually a distance between distribution of referenced language and distribution of deciphered
     * data
     *
     * @param decipheredData deciphered data
     * @return double measure of confidence, the lower it is - the higher the confidence
     */
    double measureConfidence(const ByteData &decipheredData) const;

    /**
     * @brief Try to decipher given text that was ciphered with multi byte key with specific size. Return the deciphered
     * string, multi byte key and measure of confidence. The deciphering is done by guessing each byte of the key
     * separately, using 'decipherSingle' against corresponding part of the encrypted data that was xored with this
     * specific byte of the key. The measure of confidence is a floating point number, the smaller it is - the higher
     * the confidence (can be used when comparing the results of deciphering different ciphered data)
     *
     * @param cipheredText - one byte key ciphered text, can't be empty
     * @param keySize - The specific key size to try. Assumed to be larger equal than 2
     *
     * @return deciphered text, multi-byte xor key, measure of confidence
     * @throw std::invalid_argument if range is not according to what is defined above or if cipheredText is empty
     */
    std::tuple<std::string, ByteData, double> decipherMultiKeySize(const ByteData &cipheredData,
                                                                   std::size_t keySize) const;
};

#endif