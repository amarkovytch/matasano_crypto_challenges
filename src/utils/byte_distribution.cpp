#include "byte_distribution.h"
#include <cmath>

ByteDistribution::ByteDistribution(const ByteData &byteData)
{
    auto bytes = byteData.secureData();
    double one_elm_percentage = (1.0 / static_cast<double>(bytes.size()) * static_cast<double>(100));

    for (auto b : bytes)
    {
        distributionMap_[b] += one_elm_percentage;
    }
}

double ByteDistribution::distance(const ByteDistribution &anotherDistribution) const
{
    auto iteratorThis = distributionMap_.begin();
    auto iteratorAnother = anotherDistribution.distributionMap_.begin();
    auto thisEnd = distributionMap_.end();
    auto anotherEnd = anotherDistribution.distributionMap_.end();
    double result = 0;

    while (iteratorThis != thisEnd && iteratorAnother != anotherEnd)
    {
        if (iteratorThis->first < iteratorAnother->first)
        {
            result += iteratorThis->second;
            iteratorThis++;
        }
        else if (iteratorAnother->first < iteratorThis->first)
        {
            result += iteratorAnother->second;
            iteratorAnother++;
        }
        else
        {
            result += std::abs(iteratorThis->second - iteratorAnother->second);
            iteratorThis++;
            iteratorAnother++;
        }
    }

    for (auto it = iteratorThis; it != thisEnd; it++)
    {
        result += iteratorThis->second;
    }

    for (auto it = iteratorAnother; it != anotherEnd; it++)
    {
        result += iteratorAnother->second;
    }

    return result;
}