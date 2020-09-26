#include "byte_data.h"
#include "gtest/gtest.h"

TEST(ByteDataTest, ConstructorEmpty) { ASSERT_THROW(ByteData(""), std::invalid_argument); }

TEST(ByteDataTest, ConstructorEmpty2) { ASSERT_THROW(ByteData(std::vector<std::byte>()), std::invalid_argument); }

TEST(ByteDataTest, ConstructorWrongHex) { ASSERT_THROW(ByteData("a"), std::invalid_argument); }

TEST(ByteDataTest, ConstructorWrongHex2) { ASSERT_THROW(ByteData("at"), std::invalid_argument); }

TEST(ByteDataTest, ConstructWrongBase64)
{
    ASSERT_THROW(ByteData("at", ByteData::encoding::base64), std::invalid_argument);
}

TEST(ByteDataTest, ConstructWrongBase64_2)
{
    ASSERT_THROW(ByteData("at9", ByteData::encoding::base64), std::invalid_argument);
}

TEST(ByteDataTest, ConstructWrongBase64_3)
{
    ASSERT_THROW(ByteData("=at99", ByteData::encoding::base64), std::invalid_argument);
}

TEST(ByteDataTest, ConstructWrongBase64_4)
{
    ASSERT_THROW(ByteData("at=9", ByteData::encoding::base64), std::invalid_argument);
}

TEST(ByteDataTest, ConstructBase64)
{
    ByteData bd("EjRW", ByteData::encoding::base64);
    ASSERT_EQ("123456", bd.str());
}

TEST(ByteDataTest, ConstructBase64OnePad)
{
    ByteData bd("EjRWeJA=", ByteData::encoding::base64);
    ASSERT_EQ("1234567890", bd.str());
}

TEST(ByteDataTest, ConstructBase64TwoPad)
{
    ByteData bd("EjRWeA==", ByteData::encoding::base64);
    ASSERT_EQ("12345678", bd.str());
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
    ByteData b = ByteData("123", ByteData::encoding::plain) + ByteData("456", ByteData::encoding::plain);

    ASSERT_EQ("123456", b.str(ByteData::encoding::plain));
}

TEST(ByteDataTest, OperatorPlusEq)
{
    ByteData b = ByteData("123", ByteData::encoding::plain) + ByteData("456", ByteData::encoding::plain);

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
    ByteData b3("12");

    ASSERT_TRUE(b1 == b2);
    ASSERT_FALSE(b1 != b2);

    ASSERT_TRUE(b1 != b3);
    ASSERT_FALSE(b1 == b3);

    ASSERT_TRUE(b3 != b1);
    ASSERT_FALSE(b3 == b1);
}

TEST(ByteDataTest, HammingIllegal)
{
    ByteData b1("1234");
    ByteData b2("12");

    ASSERT_THROW(b1.hamming(b2), std::invalid_argument);
}

TEST(ByteDataTest, HammingTestSelf)
{
    ByteData b1("1234");
    ByteData b2("1234");

    ASSERT_EQ(0.0, b1.hamming(b2));
}

TEST(ByteDataTest, HammingTestMax)
{
    ByteData b1("FFFF");
    ByteData b2("0000");

    ASSERT_EQ(8.0, b1.hamming(b2));
}

TEST(ByteDataTest, HammingTestWokka)
{
    ByteData b1("this is a test", ByteData::encoding::plain);
    ByteData b2("wokka wokka!!!", ByteData::encoding::plain);

    ASSERT_EQ(37.0 / 14.0, b1.hamming(b2));
}

// std::vector<ByteData> extractRows(std::size_t elmsInRow, std::size_t maxRows = 0);
TEST(ByteDataTest, ExtractRowsElmsZero)
{
    std::vector vec{std::byte{1}, std::byte{2}, std::byte{3}, std::byte{4}, std::byte{5},
                    std::byte{6}, std::byte{7}, std::byte{8}, std::byte{9}, std::byte{10}};

    ByteData bd(vec);
    ASSERT_THROW(bd.extractRows(0), std::invalid_argument);
}

TEST(ByteDataTest, ExtractRowsSanity)
{
    ByteData bd("1234567890", ByteData::encoding::plain);
    auto rows = bd.extractRows(5);

    ASSERT_EQ(2, rows.size());
    ASSERT_EQ("12345", rows.at(0).str(ByteData::encoding::plain));
    ASSERT_EQ("67890", rows.at(1).str(ByteData::encoding::plain));
}

TEST(ByteDataTest, ExtractRowsSanityMaxRows)
{
    ByteData bd("1234567890", ByteData::encoding::plain);
    auto rows = bd.extractRows(5, 2);

    ASSERT_EQ(2, rows.size());
    ASSERT_EQ("12345", rows.at(0).str(ByteData::encoding::plain));
    ASSERT_EQ("67890", rows.at(1).str(ByteData::encoding::plain));

    auto rows2 = bd.extractRows(5, 10);
    ASSERT_EQ(rows, rows2);

    auto rows3 = bd.extractRows(5, 1);
    ASSERT_EQ(1, rows3.size());
    ASSERT_EQ("12345", rows3.at(0).str(ByteData::encoding::plain));
}

TEST(ByteDataTest, ExtractRowsLargeMaxRows)
{
    ByteData bd("1234567890", ByteData::encoding::plain);
    auto rows = bd.extractRows(7, 0);

    ASSERT_EQ(2, rows.size());
    ASSERT_EQ("1234567", rows.at(0).str(ByteData::encoding::plain));
    ASSERT_EQ("890", rows.at(1).str(ByteData::encoding::plain));

    rows = bd.extractRows(10, 0);
    ASSERT_EQ(1, rows.size());
    ASSERT_EQ("1234567890", rows.at(0).str(ByteData::encoding::plain));

    rows = bd.extractRows(100, 0);
    ASSERT_EQ(1, rows.size());
    ASSERT_EQ("1234567890", rows.at(0).str(ByteData::encoding::plain));
}

TEST(ByteDataTest, ExtractColumnsZero)
{
    std::vector vec{std::byte{1}, std::byte{2}, std::byte{3}, std::byte{4}, std::byte{5},
                    std::byte{6}, std::byte{7}, std::byte{8}, std::byte{9}, std::byte{10}};

    ByteData bd(vec);
    ASSERT_THROW(bd.extractColumns(0), std::invalid_argument);
}

TEST(ByteDataTest, ExtractColsSanity)
{
    ByteData bd("1234567890", ByteData::encoding::plain);
    auto cols = bd.extractColumns(2);

    ASSERT_EQ(2, cols.size());
    ASSERT_EQ("13579", cols.at(0).str(ByteData::encoding::plain));
    ASSERT_EQ("24680", cols.at(1).str(ByteData::encoding::plain));
}

TEST(ByteDataTest, ExtractColsSanityMaxRows)
{
    ByteData bd("1234567890", ByteData::encoding::plain);
    auto cols = bd.extractColumns(2, 5);

    ASSERT_EQ(2, cols.size());
    ASSERT_EQ("13579", cols.at(0).str(ByteData::encoding::plain));
    ASSERT_EQ("24680", cols.at(1).str(ByteData::encoding::plain));

    auto cols2 = bd.extractColumns(2, 20);
    ASSERT_EQ(cols, cols2);

    auto cols3 = bd.extractColumns(2, 4);
    ASSERT_EQ(2, cols3.size());
    ASSERT_EQ("1357", cols3.at(0).str(ByteData::encoding::plain));
    ASSERT_EQ("2468", cols3.at(1).str(ByteData::encoding::plain));
}

TEST(ByteDataTest, ExtractColsLargeMaxCols)
{
    ByteData bd("1234567890", ByteData::encoding::plain);
    auto cols = bd.extractColumns(7, 0);

    ASSERT_EQ(7, cols.size());
    ASSERT_EQ("18", cols.at(0).str(ByteData::encoding::plain));
    ASSERT_EQ("29", cols.at(1).str(ByteData::encoding::plain));
    ASSERT_EQ("30", cols.at(2).str(ByteData::encoding::plain));
    ASSERT_EQ("4", cols.at(3).str(ByteData::encoding::plain));
    ASSERT_EQ("5", cols.at(4).str(ByteData::encoding::plain));
    ASSERT_EQ("6", cols.at(5).str(ByteData::encoding::plain));
    ASSERT_EQ("7", cols.at(6).str(ByteData::encoding::plain));

    cols = bd.extractColumns(10, 0);
    ASSERT_EQ(10, cols.size());
    ASSERT_EQ("1", cols.at(0).str(ByteData::encoding::plain));
    ASSERT_EQ("2", cols.at(1).str(ByteData::encoding::plain));
    ASSERT_EQ("3", cols.at(2).str(ByteData::encoding::plain));
    ASSERT_EQ("4", cols.at(3).str(ByteData::encoding::plain));
    ASSERT_EQ("5", cols.at(4).str(ByteData::encoding::plain));
    ASSERT_EQ("6", cols.at(5).str(ByteData::encoding::plain));
    ASSERT_EQ("7", cols.at(6).str(ByteData::encoding::plain));
    ASSERT_EQ("8", cols.at(7).str(ByteData::encoding::plain));
    ASSERT_EQ("9", cols.at(8).str(ByteData::encoding::plain));
    ASSERT_EQ("0", cols.at(9).str(ByteData::encoding::plain));

    auto cols2 = bd.extractColumns(100, 0);
    ASSERT_EQ(cols2, cols);
}

TEST(ByteDataTest, EqCyclicallyBothZero) { ASSERT_TRUE(ByteData().eqCyclically(ByteData())); }

TEST(ByteDataTest, EqCyclicallyOneZero)
{
    ByteData b("1234");
    ASSERT_FALSE(b.eqCyclically(ByteData()));

    ASSERT_FALSE(ByteData().eqCyclically(b));
}

TEST(ByteDataTest, EqCyclically)
{
    ByteData b1("123", ByteData::encoding::plain);
    ByteData b2("123123123", ByteData::encoding::plain);
    ByteData b3("1231231231", ByteData::encoding::plain);

    ASSERT_TRUE(b1.eqCyclically(b2));
    ASSERT_TRUE(b2.eqCyclically(b1));
    ASSERT_FALSE(b1.eqCyclically(b3));
    ASSERT_FALSE(b3.eqCyclically(b1));
}

TEST(ByteDataTest, EqCyclicallyEqual)
{
    ByteData b1("123", ByteData::encoding::plain);
    ByteData b2("123", ByteData::encoding::plain);

    ASSERT_TRUE(b1.eqCyclically(b2));
    ASSERT_TRUE(b2.eqCyclically(b1));
    ASSERT_TRUE(b1.eqCyclically(b1));
}
