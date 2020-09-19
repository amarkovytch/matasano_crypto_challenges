#include "byte_data.h"
#include "gtest/gtest.h"

TEST(ByteDataTest, ConstructorEmpty)
{
    ASSERT_THROW(ByteData(""), std::invalid_argument);
}

TEST(ByteDataTest, ConstructorEmpty2)
{
    ASSERT_THROW(ByteData(std::vector<std::byte>()), std::invalid_argument);
}

TEST(ByteDataTest, ConstructorWrongHex)
{
    ASSERT_THROW(ByteData("a"), std::invalid_argument);
}

TEST(ByteDataTest, ConstructorWrongHex2)
{
    ASSERT_THROW(ByteData("at"), std::invalid_argument);
}

TEST(ByteDataTest, StrPlain)
{
    ByteData bd("12345", ByteData::encoding::plain);
    ASSERT_EQ("12345", bd.str(ByteData::encoding::plain));
}

TEST(ByteDataTest, StrHex)
{
    ByteData bd("01234a", ByteData::encoding::hex);
    ASSERT_EQ("01234a", bd.str(ByteData::encoding::hex));
}

TEST(ByteDataTest, StrHex2)
{
    ByteData bd("414141", ByteData::encoding::hex);
    ASSERT_EQ("AAA", bd.str(ByteData::encoding::plain));
}

TEST(ByteDataTest, TestBase64NoPad)
{
    ByteData bd("123456");
    ASSERT_EQ("EjRW", bd.str(ByteData::encoding::base64));
}

TEST(ByteDataTest, TestBas64OnePad)
{
    ByteData bd("1234567890");
    ASSERT_EQ("EjRWeJA=", bd.str(ByteData::encoding::base64));
}

TEST(ByteDataTest, TestBase64TwoPad)
{
    ByteData bd("12345678");
    ASSERT_EQ("EjRWeA==", bd.str(ByteData::encoding::base64));
}

TEST(ByteDataTest, ConstructFromByte)
{
    std::byte b{0x41};
    ByteData bd(b);
    ASSERT_EQ("A", bd.str(ByteData::encoding::plain));
}

TEST(ByteDataTest, ConstructFromVector)
{
    std::byte b{0x41};

    std::vector<std::byte> vec;
    vec.push_back(b);
    vec.push_back(b);
    vec.push_back(b);

    ByteData bd(vec);
    ASSERT_EQ("AAA", bd.str(ByteData::encoding::plain));
}

TEST(ByteDataTest, DataTest)
{
    std::byte b{0x41};

    std::vector<std::byte> vec;
    vec.push_back(b);
    vec.push_back(b);
    vec.push_back(b);

    ByteData bd(vec);
    ASSERT_EQ(vec, bd.data());
}

TEST(ByteDataTest, OperatorPlus)
{
    ByteData b = ByteData("123", ByteData::encoding::plain) +
                 ByteData("456", ByteData::encoding::plain);

    ASSERT_EQ("123456", b.str(ByteData::encoding::plain));
}

TEST(ByteDataTest, OperatorPlusEq)
{
    ByteData b = ByteData("123", ByteData::encoding::plain) +
                 ByteData("456", ByteData::encoding::plain);

    b += ByteData("789", ByteData::encoding::plain);

    ASSERT_EQ("123456789", b.str(ByteData::encoding::plain));
}

TEST(ByteDataTest, OperatorXorSame)
{
    ByteData b1("1234");
    ByteData b2("1234");
    ASSERT_EQ("0000", (b1 ^ b2).str());

    ByteData b3("12");
    ByteData b4("12");
    ASSERT_EQ("00", (b3 ^ b4).str());
}

TEST(ByteDataTest, OperatorXorOneArgZero)
{
    ByteData b1("1234");
    ByteData b2("0000");
    ASSERT_EQ("1234", (b1 ^ b2).str());
    ASSERT_EQ("1234", (b2 ^ b1).str());
}

TEST(ByteDataTest, OperatorXorEqual)
{
    ByteData b1("1234");
    ByteData b2("0000");

    auto b3 = b1 ^ b2;
    ASSERT_EQ("1234", b3.str());

    b3 ^= b1;
    ASSERT_EQ("0000", b3.str());
}

TEST(ByteDataTest, OperatorXorLong)
{
    ByteData b1("abcdefabcdefabcdefabcdefabcdef");
    ByteData b2("fedcbafedcbafedcbafedcbafedcba");

    ASSERT_EQ("551155551155551155551155551155", (b1 ^ b2).str());
}

TEST(ByteDataTest, OperatorXorCyclic)
{
    ByteData b1("ab00ab00ab00ab00");
    ByteData b2("ab");

    ASSERT_EQ("00ab00ab00ab00ab", (b1 ^ b2).str());
}

TEST(ByteDataTest, OperatorXorRhsLongerThanLhs)
{
    ByteData b1("ab00ab00ab00ab00");
    ByteData b2("ab");

    ASSERT_EQ("00", (b2 ^ b1).str());
}

TEST(ByteDataTest, OperatorXorLhsDoNotDivideRhs)
{
    ByteData b1("abcc00abcc00abcc00abcc");
    ByteData b2("abccaa");

    ASSERT_EQ("0000aa0000aa0000aa0000", (b1 ^ b2).str());
}

TEST(ByteDataTest, OperatorXorCyclicOneChar)
{
    ByteData b1("61006100");

    std::byte b{0x61};
    ASSERT_EQ("00610061", (b1 ^ b).str());
}

TEST(ByteDataTest, OperatorEquals)
{
    ByteData b1("1234");
    ByteData b2("1234");

    ASSERT_TRUE(b1 == b2);
    ASSERT_FALSE(b1 != b2);
}