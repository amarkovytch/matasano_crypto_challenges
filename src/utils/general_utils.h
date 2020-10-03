#ifndef MATASANO_GENERAL_UTILS_H
#define MATASANO_GENERAL_UTILS_H

#define __STDC_WANT_LIB_EXT1__ 1
#include <string.h>

#include <cstddef>
#include <cstdint>

#define DEFAULT_COPY_MOVE_CONSTRUCTORS(class_name)                                                                     \
    class_name(const class_name &) = default;                                                                          \
    class_name(class_name &&) = default;                                                                               \
    class_name &operator=(const class_name &) = default;                                                               \
    class_name &operator=(class_name &&) = default // ; omitted on purpose

/**
 * @brief A collection of various security related utils
 */
class GeneralUtils
{
public:
    /**
     * @brief wipe given data securely (such that compiler does not optimize this out)
     *
     * @param data data to wipe
     * @param size size of data to wipe
     */
    inline static void SecureWipeData(void *data, std::size_t size)
    {
#ifdef __STDC_LIB_EXT1__
        memset_s(data, size, 0, size);
#else
        volatile uint8_t *p = reinterpret_cast<volatile uint8_t *>(data);
        for (size_t i = 0; i != size; ++i)
        {
            p[i] = 0;
        }
#endif
    }
};

#endif