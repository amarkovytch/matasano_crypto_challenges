#include "matasano_math.h"
#include "matasano_asserts.h"
#include "matasano_convert.h"

std::string Math::xorHexStrs(const std::string hex1, const std::string hex2)
{
    assertHexIsEven(hex1);
    assertHexIsEven(hex2);

    THROW_IF(hex1.length() != hex2.length(),
             hex1 + " length is not equal to the length of " + hex2,
             std::invalid_argument);

    std::string res;

    for (auto i = 0; i < hex1.length(); i += 2)
    {
        auto num1 = Convert::parseNumFromStr(hex1.substr(i, 2));
        auto num2 = Convert::parseNumFromStr(hex2.substr(i, 2));
        res += Convert::numToStr(num1 ^ num2, 2);
    }

    return res;
}