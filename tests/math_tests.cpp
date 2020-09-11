#include "matasano_math.h"
#include "gtest/gtest.h"

TEST(MathTestsXorHexStrs, TestEmpty)
{
    ASSERT_EQ("", Math::xorHexStrs("", ""));
}

TEST(MathTestsXorHexStrs, TestNonEvenHex)
{
    ASSERT_THROW(Math::xorHexStrs("123", "123"), std::invalid_argument);
}

TEST(MathTestsXorHexStrs, TestNonEqualHex)
{
    ASSERT_THROW(Math::xorHexStrs("123", "1234"), std::invalid_argument);
}

TEST(MathTestsXorHexStrs, TestNonHex)
{
    ASSERT_THROW(Math::xorHexStrs("123t", "1234"), std::invalid_argument);
    ASSERT_THROW(Math::xorHexStrs("1234", "123t"), std::invalid_argument);
    ASSERT_THROW(Math::xorHexStrs("123t", "123t"), std::invalid_argument);
}

TEST(MathTestsXorHexStrs, TestXorSame)
{
    ASSERT_EQ("00", Math::xorHexStrs("12", "12"));
    ASSERT_EQ("0000", Math::xorHexStrs("1234", "1234"));
}

TEST(MathTestsXorHexStrs, TestXorOneArgZero)
{
    ASSERT_EQ("1234", Math::xorHexStrs("1234", "0000"));
    ASSERT_EQ("1234", Math::xorHexStrs("0000", "1234"));
}

TEST(MathTestsXorHexStrs, TestXor)
{
    ASSERT_EQ("551155551155551155551155551155", Math::xorHexStrs("abcdefabcdefabcdefabcdefabcdef", "fedcbafedcbafedcbafedcbafedcba"));
}