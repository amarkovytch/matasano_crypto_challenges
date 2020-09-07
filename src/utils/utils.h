#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <stdexcept>
#include <exception>

#define VALIDATE_EQ(str1, str2)                                                                              \
        if ((str1) != (str2))                                                                                 \
        {                                                                                                    \
                throw std::runtime_error(str1 + " is not equal to " + str2 + \
                        std::string(__FILE__) + ":" + std::to_string(__LINE__));                             \
        }

class Utils
{
        public:
                /**
                 * @brief similar to assert(x)
                 *
                 * @param exp expression to be validated
                 * @throw std::runtime_error if the expression is false
                 */
                static void validate(bool exp);

                /**
                 * @brief Converts hex string to base64 string
                 *
                 * @param hex hex string (without 0x)
                 * @return base64 string
                 *
                 * @throw std::invalid_argument if hex is not in hex format
                 */
                static std::string hexToBase64(const std::string hex);

};

#endif