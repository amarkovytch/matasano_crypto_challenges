#include "general_utils.h"
#include "gtest/gtest.h"

#include <cmath>

template <class T> static double calculateMean(const T &vector)
{
    double sum = 0;
    for (const auto &elm : vector)
    {
        sum += elm;
    }

    return sum / vector.size();
}

template <class T> std::pair<double, double> static calculateMeanBounds(T low, T high, std::size_t sampleSize)
{
    double stdev = (high - low) * std::sqrt(1.0 / 12.0);
    double mean = (high + low) * 0.5;

    double upperMean = mean + stdev / std::sqrt(static_cast<double>(sampleSize));
    double lowerMean = mean - stdev / std::sqrt(static_cast<double>(sampleSize));

    return std::make_pair(lowerMean, upperMean);
}

// Disabled because the test is very long, but it only works with big samples
TEST(DISABLED_TestRandomNumber, TestUniformMean)
{
    long long int low = -1000;
    long long int high = 1000;

    std::vector<long long int> vec(10000000, 0);
    std::generate(vec.begin(), vec.end(), [low, high]() { return GeneralUtils::randomNum<long long int>(low, high); });

    auto mean = calculateMean(vec);
    auto [lowerMean, upperMean] = calculateMeanBounds(low, high, vec.size());

    ASSERT_GE(mean, lowerMean);
    ASSERT_LE(mean, upperMean);
}
// Disabled because the test is very long, but it only works with big samples
TEST(DISABLED_TestRandomVector, TestUniformMean)
{
    auto data = GeneralUtils::randomData(10000000);
    ASSERT_EQ(100000, data.size());

    std::uint8_t low = 0;
    std::uint8_t high = 255;

    double mean = calculateMean(data.secureData());
    auto [lowerMean, upperMean] = calculateMeanBounds(low, high, data.size());

    ASSERT_GE(mean, lowerMean);
    ASSERT_LE(mean, upperMean);
}

TEST(TestRandomNumber, TestEqualRanges)
{
    ASSERT_EQ(0, GeneralUtils::randomNum(0, 0));
    ASSERT_EQ(5, GeneralUtils::randomNum(5, 5));
}

TEST(TestRandomNumber, TestEmptyVector) { ASSERT_EQ(0, GeneralUtils::randomData(0).size()); }
