#include "matasano_convert.h"
#include "matasano_asserts.h"

#include <charconv>
#include <iomanip>
#include <sstream>

char Convert::numToBase64(unsigned int num)
{
    LOGIC_ASSERT(num <= 64);

    if (num <= 25)
    {
        return num + 'A';
    }

    if (num <= 51)
    {
        return (num - 26) + 'a';
    }

    if (num <= 61)
    {
        return (num - 52) + '0';
    }

    if (num == 62)
    {
        return '+';
    }

    // num == 63
    return '/';
}

std::string Convert::hex3ToBase64_4(const std::string hex)
{
    LOGIC_ASSERT(hex.length() == 6);

    unsigned long number = parseNumFromStr(hex, 16);

    auto octal = numToStr(number, 8, 8);

    LOGIC_ASSERT(octal.length() == 8);

    std::string base64;
    for (unsigned int i = 0; i < octal.length(); i += 2)
    {
        auto num = parseNumFromStr(octal.substr(i, 2), 8);
        base64 += numToBase64(num);
    }

    return base64;
}

unsigned long long Convert::parseNumFromStr(const std::string str, int base)
{
    auto SUCCESS = std::errc();
    unsigned long long result = 0;
    auto [ptr, ec] =
        std::from_chars(str.c_str(), str.c_str() + str.length(), result, base);
    if (ec != SUCCESS || ptr != (str.c_str() + str.length()))
    {
        throw std::invalid_argument("can't parse " + std::to_string(base) +
                                    " number from " + str);
    }

    return result;
}

std::string Convert::padWith(const std::string str, const std::string pad,
                             std::size_t iterations)
{
    auto padded = str;

    for (unsigned int i = 0; i < iterations; i++)
    {
        padded += pad;
    }

    return padded;
}

std::string Convert::numToStr(unsigned long long num, std::size_t min_width, int base)
{
    std::ostringstream os;
    os << std::setbase(base) << std::setfill('0') << std::setw(min_width) << num;

    return os.str();
}