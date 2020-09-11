#ifndef ASSERTS_H
#define ASSERTS_H

/**
 * @brief Throws a given exception with given message if cond is true
 */
#define THROW_IF(cond, message, exception)  \
        if ((cond))                         \
        {                                   \
                throw exception((message)); \
        }

/**
 * @brief Throws runtime_error exception if 2 given strings are not equal.
 * Used to validate expected output in exercises
 */
#define VALIDATE_EQ(str1, str2) \
        THROW_IF((str1) != (str2), str1 + " is not equal to " + str2 + FILE_LINE, std::runtime_error);

/**
 * @brief Throws std::logic_error if given condition is violated
 * Used to validate internal logic
 */
#define ASSERT(cond) \
        THROW_IF(!(cond), std::string(#cond) + " is violated " + FILE_LINE, std::logic_error);

/**
 * @brief Generates __FILE__:__LINE__ string
 * Used in internal assertions
 */
#define FILE_LINE std::string(__FILE__) + ":" + std::to_string(__LINE__)

#endif