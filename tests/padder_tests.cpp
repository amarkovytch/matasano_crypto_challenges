#include "padder.h"
#include "gtest/gtest.h"

TEST(PadderTests, PadderZero)
{
    ByteData b("123456");
    ASSERT_EQ(b, Padder::pad(b, 0));
}

TEST(PadderTests, PadderTest)
{
    ByteData b("123456");
    ByteData padded = b + std::vector(4, std::byte{4});
    ASSERT_EQ(padded, Padder::pad(b, 4));
}

TEST(PadderTests, PadderBlock)
{
    ByteData b("123456");
    ByteData padded = b + std::vector(5, std::byte{5});
    ASSERT_EQ(padded, Padder::padToBlockSize(b, 8));
}

TEST(PadderTests, PadderBlockFullSize)
{
    ByteData b("1122334455667788");
    ByteData padded = b + std::vector(8, std::byte{8});
    ASSERT_EQ(padded, Padder::padToBlockSize(b, 8));
}

TEST(PadderTests, PadderBlockZeroSize)
{
    ByteData b;
    ByteData padded = b + std::vector(8, std::byte{8});
    ASSERT_EQ(padded, Padder::padToBlockSize(b, 8));
}

TEST(PadderTests, PadderBlockWrongSize)
{
    ByteData b("1122334455667788");
    ASSERT_THROW(Padder::padToBlockSize(b, 4), std::invalid_argument);
}

TEST(PadderTests, RemovePadding)
{
    ByteData b("123456");
    auto padded = Padder::pad(b, 4);
    auto paddingRemoved = Padder::removePadding(padded);
    ASSERT_EQ(b, paddingRemoved);
}

TEST(PadderTests, RemovePaddingWrongNumber)
{
    ByteData b("123456");
    ASSERT_THROW(Padder::removePadding(b + std::byte{255}), std::invalid_argument);
}

TEST(PadderTests, RemovePaddingWrongPadding)
{
    ByteData b("123456", ByteData::Encoding::plain);
    ASSERT_THROW(Padder::removePadding(b + std::byte{3} + std::byte{3}), std::invalid_argument);
}
