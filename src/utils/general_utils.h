#ifndef MATASANO_GENERAL_UTILS_H
#define MATASANO_GENERAL_UTILS_H

#include <cstddef>
#include <cstdint>
#include <random>
#include <string.h>

#include "byte_data.h"
#include "matasano_asserts.h"

#define DEFAULT_COPY_MOVE_CONSTRUCTORS(class_name)                                                                     \
    class_name(const class_name &) = default;                                                                          \
    class_name(class_name &&) = default;                                                                               \
    class_name &operator=(const class_name &) = default;                                                               \
    class_name &operator=(class_name &&) = default // ; omitted on purpose

/**
 * @brief A collection of various utils
 */
namespace GeneralUtils
{
/**
 * @brief Generates random number within the given range [from, to]
 *
 * @param from from and including this number
 * @param to up to and including this number
 * @return random number
 *
 * @throw std::invalid_argument if to < from
 */
template <typename T> requires std::is_integral_v<T> T randomNum(T from, T to)
{
    THROW_IF(to < from, "wrong range", std::invalid_argument);

    auto distribution_ = std::uniform_int_distribution<T>(from, to);
    auto random_engine_ = std::mt19937{std::random_device{}()};
    return distribution_(random_engine_);
}

/**
 * @brief Creates ByteData with length of 'length' filled with random bytes
 *
 * @param length the length of the random ByteData
 * @return ByteData the random ByteData
 */
ByteData randomData(std::size_t length);

} // namespace GeneralUtils

#endif