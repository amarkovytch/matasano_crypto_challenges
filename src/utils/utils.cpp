#include <utils.h>
#include <stdexcept>
#include <sstream>
#include <charconv>
#include <iomanip>

char Utils::numToBase64(unsigned int num)
{
        ASSERT(num <= 64);

        if (num <= 25) {
                return num + 'A';
        }

        if (num <= 51) {
                return (num  - 26) + 'a';
        }

        if (num <= 61) {
                return (num - 52) + '0';
        }

        if (num == 62) {
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
                auto num = parseIntFromStr(octal.substr(i, 2), 8);
                base64 += numToBase64(num);
        }

        ASSERT(base64.length() == 4);
        return base64;
}

unsigned long long Utils::parseIntFromStr(const std::string str, int base)
{
        auto SUCCESS = std::errc();
        unsigned long long result = 0;
        auto [ptr, ec] = std::from_chars(str.c_str(), str.c_str() + str.length(), result, base);
        if (ec != SUCCESS || ptr != (str.c_str() + str.length())) {
                throw std::invalid_argument("can't parse " + std::to_string(base) +
                      " number from " + str);
        }

        return result;
}

std::string Utils::padWith(const std::string str, const std::string pad, size_t iterations)
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
        unsigned long number = parseIntFromStr(hex, 16);

        os << std::oct << std::setfill('0') << std::setw(8) << number;

        auto res = os.str();
        ASSERT(res.length() == 8);

        return res;
}

// The principle is simple: translate every 3 hex bytes (6 char) to octal string (of length 4)
// If hex string is not multiply of 3, add needed 00 padding, in the end, remove the same number of padded bytes
// and place '=' * number of padded bytes instead
// Look at https://en.wikipedia.org/wiki/Base64 for more info
std::string Utils::hexToBase64(const std::string hex)
{
        std::string result;

        if (hex.length() % 2 != 0) {
                throw std::invalid_argument(hex + " has uneven length");
        }

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
