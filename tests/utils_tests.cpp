#include <iostream>
#include <stdexcept>

#include "utils.h"
#include "gtest/gtest.h"

TEST(UtilsTestsBase64, TestEmpty)
{
    auto empty = Utils::hexToBase64("");
    ASSERT_EQ(empty, "");
}

TEST(UtilsTestsBase64, TestHexWithWrongLength)
{
    ASSERT_THROW(Utils::hexToBase64("abc"), std::invalid_argument);
}

TEST(UtilsTestsBase64, TestNoPad)
{
    ASSERT_EQ("EjRW", Utils::hexToBase64("123456"));
}

TEST(UtilsTestsBase64, Test1Pad)
{
    ASSERT_EQ("EjRWeJA=", Utils::hexToBase64("1234567890"));
}

TEST(UtilsTestsBase64, Test2Pad)
{
    ASSERT_EQ("EjRWeA==", Utils::hexToBase64("12345678"));
}

TEST(UtilsTestsParseNum, TestEmpty)
{
    ASSERT_THROW(Utils::parseNumFromStr(""), std::invalid_argument);
}

TEST(UtilsTestsParseNum, TestWrongStr)
{
    ASSERT_THROW(Utils::parseNumFromStr("yyy"), std::invalid_argument);
}

TEST(UtilsTestsParseNum, TestWrongStr2)
{
    ASSERT_THROW(Utils::parseNumFromStr("aaa", 10), std::invalid_argument);
}

TEST(UtilsTestsParseNum, Test10)
{
    ASSERT_EQ(12345, Utils::parseNumFromStr("12345", 10));
}

TEST(UtilsTestsParseNum, Test16)
{
    ASSERT_EQ(0xabcd, Utils::parseNumFromStr("abcd"));
}

TEST(UtilsTestsParseNum, Test8)
{
    // prefix '0' is for octals
    ASSERT_EQ(0777, Utils::parseNumFromStr("777", 8));
}

TEST(UtilsTestsnumToStr, Zero)
{
    ASSERT_EQ("0", Utils::numToStr(0));
}

TEST(UtilsTestsNumToStr, Hex)
{
    ASSERT_EQ("1234a", Utils::numToStr(0x1234a));
}

TEST(UtilsTestsNumToStr, HexWithLeadingZero)
{
    ASSERT_EQ("01234a", Utils::numToStr(0x1234a, 6));
}

TEST(UtilsTestsNumToStr, Decimal)
{
    ASSERT_EQ("012345", Utils::numToStr(12345, 6, 10));
}

TEST(UtilsTestsNumToStr, NumFromStringAndBack)
{
    auto str = "aaaa";
    auto num = Utils::parseNumFromStr(str);
    ASSERT_EQ(str, Utils::numToStr(num));
}

TEST(UtilsTestsXorHexStrs, TestEmpty)
{
    ASSERT_EQ("", Utils::xorHexStrs("", ""));
}

TEST(UtilsTestsXorHexStrs, TestNonEvenHex)
{
    ASSERT_THROW(Utils::xorHexStrs("123", "123"), std::invalid_argument);
}

TEST(UtilsTestsXorHexStrs, TestNonEqualHex)
{
    ASSERT_THROW(Utils::xorHexStrs("123", "1234"), std::invalid_argument);
}

TEST(UtilsTestsXorHexStrs, TestNonHex)
{
    ASSERT_THROW(Utils::xorHexStrs("123t", "1234"), std::invalid_argument);
    ASSERT_THROW(Utils::xorHexStrs("1234", "123t"), std::invalid_argument);
    ASSERT_THROW(Utils::xorHexStrs("123t", "123t"), std::invalid_argument);
}

TEST(UtilsTestsXorHexStrs, TestXorSame)
{
    ASSERT_EQ("00", Utils::xorHexStrs("12", "12"));
    ASSERT_EQ("0000", Utils::xorHexStrs("1234", "1234"));
}

TEST(UtilsTestsXorHexStrs, TestXorOneArgZero)
{
    ASSERT_EQ("1234", Utils::xorHexStrs("1234", "0000"));
    ASSERT_EQ("1234", Utils::xorHexStrs("0000", "1234"));
}

TEST(UtilsTestsXorHexStrs, TestXor)
{
    ASSERT_EQ("551155551155551155551155551155", Utils::xorHexStrs("abcdefabcdefabcdefabcdefabcdef", "fedcbafedcbafedcbafedcbafedcba"));
}