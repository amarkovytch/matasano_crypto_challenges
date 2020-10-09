#include "byte_data.h"
#include "byte_distribution.h"
#include "gtest/gtest.h"

TEST(ByteDistributionTest, DistributionTest)
{
    ByteData bytes("aabc", ByteData::Encoding::plain);
    ByteDistribution distribution(bytes);

    ASSERT_EQ(3, distribution.size());

    ASSERT_EQ(50.0, distribution.at(std::uint8_t{'a'}));
    ASSERT_EQ(25.0, distribution.at(std::uint8_t{'b'}));
    ASSERT_EQ(25.0, distribution.at(std::uint8_t{'c'}));
    ASSERT_EQ(0, distribution.at(std::uint8_t{'h'}));
}

TEST(ByteDistributionTest, DistributionTestRemove)
{
    ByteData bytes("aabc", ByteData::Encoding::plain);
    ByteDistribution distribution(bytes);

    ASSERT_EQ(3, distribution.size());

    distribution.erase(std::uint8_t{'r'});
    distribution.erase(std::uint8_t{'a'});

    ASSERT_EQ(2, distribution.size());

    ASSERT_EQ(0, distribution.at(std::uint8_t{'a'}));
    ASSERT_EQ(25.0, distribution.at(std::uint8_t{'b'}));
    ASSERT_EQ(25.0, distribution.at(std::uint8_t{'c'}));
    ASSERT_EQ(0, distribution.at(std::uint8_t{'h'}));
}

TEST(ByteDistributionTest, DistributionTestFraction)
{
    ByteData bytes("caaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", ByteData::Encoding::plain);
    ByteDistribution distribution(bytes);

    ASSERT_EQ(2.5, distribution.at(std::uint8_t{'c'}));
}

TEST(ByteDistributionTest, DistanceSame)
{
    ByteData bytes("aabc", ByteData::Encoding::plain);
    ByteDistribution distribution(bytes);

    ByteData bytes2("aabc", ByteData::Encoding::plain);
    ByteDistribution distribution2(bytes);

    ASSERT_EQ(0, distribution.distance(distribution2));
    ASSERT_EQ(0, distribution.distance(distribution));
}

TEST(ByteDistributionTest, DistanceDifferentSlightly)
{
    ByteData bytes("aabc", ByteData::Encoding::plain);
    ByteDistribution distribution(bytes);

    ByteData bytes2("aaec", ByteData::Encoding::plain);
    ByteDistribution distribution2(bytes2);

    ASSERT_EQ(50.0, distribution.distance(distribution2));
}

TEST(ByteDistributionTest, DistanceDifferentCompletely)
{
    ByteData bytes("bc", ByteData::Encoding::plain);
    ByteDistribution distribution(bytes);

    ByteData bytes2("aefg", ByteData::Encoding::plain);
    ByteDistribution distribution2(bytes2);

    ASSERT_EQ(200.0, distribution.distance(distribution2));
}