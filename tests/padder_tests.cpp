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
    ByteData padded = b + std::byte{4} + std::byte{4} + std::byte{4} + std::byte{4};
    ASSERT_EQ(padded, Padder::pad(b, 4));
}
