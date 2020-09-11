#include "matasano_convert.h"
#include "gtest/gtest.h"

TEST(ConvertTestsBase64, TestEmpty)
{
    auto empty = Convert::hexToBase64("");
    ASSERT_EQ(empty, "");
}

TEST(ConvertTestsBase64, TestHexWithWrongLength)
{
    ASSERT_THROW(Convert::hexToBase64("abc"), std::invalid_argument);
}

TEST(ConvertTestsBase64, TestNoPad)
{
    ASSERT_EQ("EjRW", Convert::hexToBase64("123456"));
}

TEST(ConvertTestsBase64, Test1Pad)
{
    ASSERT_EQ("EjRWeJA=", Convert::hexToBase64("1234567890"));
}

TEST(ConvertTestsBase64, Test2Pad)
{
    ASSERT_EQ("EjRWeA==", Convert::hexToBase64("12345678"));
}

TEST(ConvertTestsParseNum, TestEmpty)
{
    ASSERT_THROW(Convert::parseNumFromStr(""), std::invalid_argument);
}

TEST(ConvertTestsParseNum, TestWrongStr)
{
    ASSERT_THROW(Convert::parseNumFromStr("yyy"), std::invalid_argument);
}

TEST(ConvertTestsParseNum, TestWrongStr2)
{
    ASSERT_THROW(Convert::parseNumFromStr("aaa", 10), std::invalid_argument);
}

TEST(ConvertTestsParseNum, Test10)
{
    ASSERT_EQ(12345, Convert::parseNumFromStr("12345", 10));
}

TEST(ConvertTestsParseNum, Test16)
{
    ASSERT_EQ(0xabcd, Convert::parseNumFromStr("abcd"));
}

TEST(ConvertTestsParseNum, Test8)
{
    // prefix '0' is for octals
    ASSERT_EQ(0777, Convert::parseNumFromStr("777", 8));
}

TEST(ConvertTestsnumToStr, Zero)
{
    ASSERT_EQ("0", Convert::numToStr(0));
}

TEST(ConvertTestsNumToStr, Hex)
{
    ASSERT_EQ("1234a", Convert::numToStr(0x1234a));
}

TEST(ConvertTestsNumToStr, HexWithLeadingZero)
{
    ASSERT_EQ("01234a", Convert::numToStr(0x1234a, 6));
}

TEST(ConvertTestsNumToStr, Decimal)
{
    ASSERT_EQ("012345", Convert::numToStr(12345, 6, 10));
}

TEST(ConvertTestsNumToStr, NumFromStringAndBack)
{
    auto str = "aaaa";
    auto num = Convert::parseNumFromStr(str);
    ASSERT_EQ(str, Convert::numToStr(num));
}