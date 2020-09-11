#include <asserts.h>
#include <utils.h>

#include <charconv>
#include <iomanip>
#include <sstream>
#include <stdexcept>

char Utils::numToBase64(unsigned int num)
{
    ASSERT(num <= 64);

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

std::string Utils::octal4ToBase64_4(const std::string octal)
{
    ASSERT(octal.length() == 8);

    std::string base64;
    for (int i = 0; i < octal.length(); i += 2)
    {
        auto num = parseNumFromStr(octal.substr(i, 2), 8);
        base64 += numToBase64(num);
    }

    ASSERT(base64.length() == 4);
    return base64;
}

unsigned long long Utils::parseNumFromStr(const std::string str, int base)
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

std::string Utils::padWith(const std::string str, const std::string pad,
                           size_t iterations)
{
    auto padded = str;

    for (int i = 0; i < iterations; i++)
    {
        padded += pad;
    }

    return padded;
}

std::string Utils::hex3To4Octal(const std::string hex)
{
    ASSERT(hex.length() == 6);

    std::ostringstream os;
    unsigned long number = parseNumFromStr(hex, 16);

    auto res = numToStr(number, 8, 8);

    ASSERT(res.length() == 8);

    return res;
}

void Utils::assertHexIsEven(const std::string hex)
{
    THROW_IF(hex.length() % 2 != 0, hex + " has uneven length",
             std::invalid_argument);
}

// The principle is simple: translate every 3 hex bytes (6 char) to octal string
// (of length 4) If hex string is not multiply of 3, add needed 00 padding, in
// the end, remove the same number of padded bytes and place '=' * number of
// padded bytes instead Look at https://en.wikipedia.org/wiki/Base64 for more
// info
std::string Utils::hexToBase64(const std::string hex)
{
    std::string result;

    assertHexIsEven(hex);

    auto numBytes = hex.length() / 2;
    unsigned char bytesToPad = (3 - numBytes % 3) % 3;

    // this is to guarantee that hex string is a multiply of triples of bytes
    auto padded_hex = padWith(hex, "00", bytesToPad);

    for (auto i = 0; i < hex.length(); i += 6)
    {
        auto octal4 = hex3To4Octal(padded_hex.substr(i, 6));
        auto base64_4 = octal4ToBase64_4(octal4);
        result += base64_4;
    }

    // remove the padding symbols
    result = result.erase(result.length() - bytesToPad, bytesToPad);

    return padWith(result, "=", bytesToPad);
}

std::string Utils::numToStr(unsigned long long num, size_t min_width, int base)
{
    std::ostringstream os;
    os << std::setbase(base) << std::setfill('0') << std::setw(min_width) << num;

    return os.str();
}

std::string Utils::xorHexStrs(const std::string hex1, const std::string hex2)
{
    assertHexIsEven(hex1);
    assertHexIsEven(hex2);

    THROW_IF(hex1.length() != hex2.length(),
             hex1 + " length is not equal to the length of " + hex2,
             std::invalid_argument);

    std::string res;

    for (auto i = 0; i < hex1.length(); i += 2)
    {
        auto num1 = parseNumFromStr(hex1.substr(i, 2));
        auto num2 = parseNumFromStr(hex2.substr(i, 2));
        res += numToStr(num1 ^ num2, 2);
    }

    return res;
}