#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <stdexcept>
#include <exception>
#include <tuple>

#define VALIDATE_EQ(str1, str2)                                                                   \
        if ((str1) != (str2))                                                                     \
        {                                                                                         \
                throw std::runtime_error(str1 + " is not equal to " + str2 +                      \
                                         std::string(__FILE__) + ":" + std::to_string(__LINE__)); \
        }

#define ASSERT(cond)                                                                            \
        if (!(cond))                                                                              \
        {                                                                                       \
                throw std::logic_error(std::string(#cond) + " is violated " +                   \
                                       std::string(__FILE__) + ":" + std::to_string(__LINE__)); \
        }

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
                 * @param base base, 10 by default
                 *
                 * @return unsigned long
                 *
                 * @throw std::invalid_argument if hex is not in hex format or the
                 * number is out of range
                 */
                static unsigned long long parseIntFromStr(const std::string str, int base = 10);

        private:
                /**
                 * @brief Converts hex string of 3 bytes (6 length) to octal string of 4 octets
                 *
                 * @param hex Hex string (without 0x). Assumes the length is 6
                 * @return octal string
                 *
                 * @throw std::invalid_argument if hex is not in hex format
                 */
                static std::string hex3To4Octal(const std::string hex);

                /**
                 * @brief Converts octal string of length 4 octets (8 length) to base64 string of 4 symbols
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
                static std::string
                padWith(const std::string str, const std::string pad, size_t iterations);

                /**
                 * @brief convert a number into a single bas64 char
                 * assumes the number is in range(0, 63)
                 *
                 * @param num number to convert
                 * @return base64 char
                 */
                static char numToBase64(unsigned int num);
};

#endif