#ifndef MATASANO_CONVERT_H
#define MATASANO_CONVERT_H

#include <exception>
#include <stdexcept>
#include <string>

/**
 * @brief Collection of various convertes
 */
class Convert
{
public:
    /**
     * @brief Parses unsigned long long integer from numerical string
     *
     * @param str string
     * @param base base, 16 by default. Should be in this range [2, 36]
     *
     * @return unsigned long
     *
     * @throw std::invalid_argument if str is not in 'base' format or the
     * number is out of range
     */
    static unsigned long long parseNumFromStr(const std::string &str, int base = 16);

    /**
     * @brief returns string with a given unsigned number
     *
     * @param num unsigned long long number to convert to string
     * @param min_width number minimal width (will be prepended with '0's if number width is less that that)
     * @param base base, 16 by default. Should be 8, 10, 16. Any other value is treated as 10
     *
     * @return std::string resulting string
     */
    static std::string numToStr(unsigned long long num, int min_width = 0, int base = 16);

    /**
     * @brief Converts hex string of 3 bytes (6 length) to base64
     * string of 4 symbols
     *
     * @param hex Hex string (without 0x). Assumes the length is 6
     * @return base64 string
     *
     * @throw std::invalid_argument if hex is not in hex format
     */
    static std::string hex3ToBase64_4(const std::string &hex);

    /**
     * @brief Converts base64 string of 4 bytes to hex string of 3 symbol
     *
     * @param base64 base64 string. Assumes the length is 4
     * @return hex string
     *
     * @throw std::invalid_argument if base64 is not in base64 format
     */
    static std::string base64_4ToHex3(const std::string &base64);

    /**
     * @brief pad the given string the the given amount of pad string iterations
     *
     * @param str string to pad
     * @param pad string to pad with
     * @param iterations number of times to repead the pad
     * @return padded string
     */
    static std::string padWith(const std::string &str, const std::string &pad, std::size_t iterations);

private:
    /**
     * @brief convert a number into a single bas64 char
     * assumes the number is in range[0, 63]
     *
     * @param num number to convert
     * @return base64 char
     */
    static char numToBase64(unsigned long long num);

    /**
     * @brief convert a base64 char into number
     * assumes char is a valid bas64 char
     *
     * @param base64 char to convert
     * @return number in range [0, 63]
     *
     * @throw std::invalid_argument if base64 is not in base64 format
     */
    static unsigned int base64ToNum(char base64);
};

#endif