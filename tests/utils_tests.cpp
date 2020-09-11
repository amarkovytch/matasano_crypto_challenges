#include <iostream>
#include <stdexcept>

#include "gtest/gtest.h"
#include "utils.h"

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

TEST(UtilsTestsParseInt, TestEmpty)
{
        ASSERT_THROW(Utils::parseIntFromStr(""), std::invalid_argument);
}

TEST(UtilsTestsParseInt, TestWrongStr)
{
        ASSERT_THROW(Utils::parseIntFromStr("yyy"), std::invalid_argument);
}

TEST(UtilsTestsParseInt, TestWrongStr2)
{
        ASSERT_THROW(Utils::parseIntFromStr("aaa", 10), std::invalid_argument);
}

TEST(UtilsTestsParseInt, Test10)
{
        ASSERT_EQ(12345, Utils::parseIntFromStr("12345"));
}

TEST(UtilsTestsParseInt, Test16)
{
        ASSERT_EQ(0xabcd, Utils::parseIntFromStr("abcd", 16));
}

TEST(UtilsTestsParseInt, Test8)
{
        // prefix '0' is for octals
        ASSERT_EQ(0777, Utils::parseIntFromStr("777", 8));
}
