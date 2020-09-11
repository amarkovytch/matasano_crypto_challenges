#ifndef MATASANO_MATH_H
#define MATASANO_MATH_H

#include <string>

/**
 * @brief A collection of varios mathematical operations
 *
 */
class Math
{
public:
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
};

#endif