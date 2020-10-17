#ifndef MATASANO_ASSERTS_H
#define MATASANO_ASSERTS_H

#include <ostream>
#include <string>

/**
 * @brief Throws a given exception with given message if cond is true
 */
#define THROW_IF(cond, message, exception)                                                                             \
    if ((cond))                                                                                                        \
    {                                                                                                                  \
        throw exception((message));                                                                                    \
    }

/**
 * @brief Throws runtime_error exception if 2 given values are not equal.
 * Used to validate expected output in exercises
 */
#define VALIDATE_EQ(val1, val2)                                                                                        \
    THROW_IF((val1) != (val2), std::to_string(val1) + " is not equal to " + std::to_string(val2) + FILE_LINE,          \
             std::runtime_error);

/**
 * @brief Throws runtime_error exception if 2 given strings are not equal.
 * Used to validate expected output in exercises
 */
#define VALIDATE_STR_EQ(str1, str2)                                                                                    \
    THROW_IF((str1) != (str2), str1 + " is not equal to " + str2 + FILE_LINE, std::runtime_error);

/**
 * @brief Throws runtime_error exception if 2 given strings are equal.
 * Used to validate expected output in exercises
 */
#define VALIDATE_STR_NOT_EQ(str1, str2)                                                                                \
    THROW_IF((str1) == (str2), str1 + " is equal to " + str2 + FILE_LINE, std::runtime_error);

/**
 * @brief Throws runtime_error exception if 2 given strings are not equal.
 * Used to validate expected output in exercises
 */
#define VALIDATE_NOT_NULL(val)                                                                                         \
    THROW_IF((val) == nullptr, std::string(#val) + " is null" + FILE_LINE, std::runtime_error);

/**
 * @brief Throws runtime_error exception if val is false
 * Used to validate expected output in exercises
 */
#define VALIDATE_TRUE(val) THROW_IF(!(val), std::string(#val) + " is false" + FILE_LINE, std::runtime_error);

/**
 * @brief Throws runtime_error exception if val is true
 * Used to validate expected output in exercises
 */
#define VALIDATE_FALSE(val) VALIDATE_TRUE(!(val))

/**
 * @brief Throws std::logic_error if given condition is violated
 * Used to validate internal logic
 */
#define LOGIC_ASSERT(cond) THROW_IF(!(cond), std::string(#cond) + " is violated " + FILE_LINE, std::logic_error);

/**
 * @brief Throws std::logic_error if this point in code is reached
 */
#define LOGIC_SHOULD_NOT_REACH_THAT_POINT()                                                                            \
    THROW_IF(true, std::string("this point in code should not have been reached ...") + FILE_LINE, std::logic_error);

/**
 * @brief Generates __FILE__:__LINE__ string
 * Used in internal assertions
 */
#define FILE_LINE "\n" + std::string(__FILE__) + ":" + std::to_string(__LINE__)

#endif