#ifndef UTILS_H
#define UTILS_H

#include <exception>
#include <stdexcept>
#include <string>
#include <tuple>

class Utils
{
public:
    /**
     * @brief Converts hex string to base64 string
     *
     * @param hex Hex string (without 0x)
     * @return base64 string
     *
     * @throw std::invalid_argument if hex is not in hex format
     */
    static std::string hexToBase64(const std::string hex);

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
    static unsigned long long parseNumFromStr(const std::string str,
                                              int base = 16);

    /**
     * @brief returns string with a given unsigned number
     *
     * @param num unsigned long long number to convert to string
     * @param min_width number minimal width (will be prepended with '0's if number width is less that that)
     * @param base base, 16 by default. Should be 8, 10, 16. Any other value is treated as 10
     *
     * @return std::string resulting string
     */
    static std::string numToStr(unsigned long long num, size_t min_width = 0, int base = 16);

    /**
     * @brief Returns xor of 2 given hex strings
     *
     * @param hex1 hex string1
     * @param hex2 hex string2
     *
     * @return std::string the xored result
     *
     * @throw std::invalid_argument if either of given arguments is not in hex
     * format or if they have not equal length
     */
    static std::string xorHexStrs(const std::string hex1,
                                  const std::string hex2);

private:
    /**
     * @brief Converts hex string of 3 bytes (6 length) to octal string of 4
     * octets
     *
     * @param hex Hex string (without 0x). Assumes the length is 6
     * @return octal string
     *
     * @throw std::invalid_argument if hex is not in hex format
     */
    static std::string hex3To4Octal(const std::string hex);

    /**
     * @brief Converts octal string of length 4 octets (8 length) to base64
     * string of 4 symbols
     *
     * @param octal string. Assumes the length of 8
     * @return base64 string
     */
    static std::string octal4ToBase64_4(const std::string octal);

    /**
     * @brief pad the given string the the given amount of pad string iterations
     *
     * @param str string to pad
     * @param pad string to pad with
     * @param iterations number of times to repead the pad
     * @return padded string
     */
    static std::string padWith(const std::string str, const std::string pad,
                               size_t iterations);

    /**
     * @brief convert a number into a single bas64 char
     * assumes the number is in range(0, 63)
     *
     * @param num number to convert
     * @return base64 char
     */
    static char numToBase64(unsigned int num);

    /**
     * @brief throws an exception if hex has uneven length
     *
     * @param hex hex string to validate
     * @throw std::invalid_argument if has has uneven length
     */
    static void assertHexIsEven(const std::string hex);
};

#endif