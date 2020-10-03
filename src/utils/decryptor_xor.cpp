#include "decryptor_xor.h"
#include "byte_distribution.h"
#include "matasano_asserts.h"
#include <limits>
#include <queue>
#include <tuple>

DecryptorXor::DecryptorXor(const std::string &referenceLanguageData)
    : referenceLanguage_(ByteDistribution(ByteData(referenceLanguageData, ByteData::Encoding::plain)))
{
}

std::tuple<std::string, std::byte, double> DecryptorXor::decipherSingle(const ByteData &cipheredData) const
{
    auto candidate = std::make_tuple(cipheredData, std::byte{0}, std::numeric_limits<double>::max());
    unsigned char curKey = 0;
    do
    {
        auto curDecipher = cipheredData ^ std::byte{curKey};

        auto curConfidence = measureConfidence(curDecipher);

        if (curConfidence < std::get<2>(candidate))
        {
            candidate = std::make_tuple(curDecipher, std::byte{curKey}, curConfidence);
            // a little optimization for the trivial case, if we have exact match, no point in iterating further
            if (0.0 == curConfidence)
            {
                break;
            }
        }

    } while (++curKey != 0);

    return std::make_tuple(std::get<0>(candidate).str(ByteData::Encoding::plain), std::get<1>(candidate),
                           std::get<2>(candidate));
}

std::tuple<std::string, ByteData, double>
DecryptorXor::decipherMulti(const ByteData &cipheredData, const std::pair<std::size_t, std::size_t> &keySizeRange) const
{
    auto [keyMin, keyMax] = keySizeRange;

    THROW_IF(keyMin < 2, "minimal key size can't be less than 2", std::invalid_argument);
    THROW_IF(keyMax < keyMin, "maximum key size should be bigger than minumum", std::invalid_argument);

    auto keySizes = guessKeySize(cipheredData, keySizeRange);

    auto result = std::make_tuple(std::string(), ByteData(), std::numeric_limits<double>::max());
    for (auto keySize : keySizes)
    {
        auto [curDecipher, currKey, currConfidence] = decipherMultiKeySize(cipheredData, keySize);
        if (currConfidence <= std::get<2>(result))
        {
            // the keys of keys that are multiple of the original key, always prefer the smaller one
            if (currConfidence == std::get<2>(result))
            {
                if (currKey.size() < std::get<1>(result).size())
                {
                    result = std::make_tuple(curDecipher, currKey, currConfidence);
                }
            }
            else
            {
                result = std::make_tuple(curDecipher, currKey, currConfidence);
            }
        }
    }

    return result;
}

std::vector<std::size_t> DecryptorXor::guessKeySize(const ByteData &cipheredData,
                                                    const std::pair<std::size_t, std::size_t> &keySizeRange,
                                                    std::size_t numberOfCandidatesToReturn) const
{
    auto [startKeyRange, endKeyRange] = keySizeRange;

    auto cmpKeyHummingPairs = [](const std::pair<std::size_t, double> &left,
                                 const std::pair<std::size_t, double> &right) {
        return std::get<1>(left) > std::get<1>(right);
    };

    std::priority_queue<std::pair<std::size_t, double>, std::vector<std::pair<std::size_t, double>>,
                        decltype(cmpKeyHummingPairs)>
        bestKeys(cmpKeyHummingPairs);

    for (auto tryKeySize = startKeyRange; tryKeySize <= endKeyRange; tryKeySize++)
    {
        auto rows = cipheredData.extractRows(tryKeySize);

        // the last row could have different size, let's ignore it
        if (rows.front().size() != rows.back().size())
        {
            rows.pop_back();
        }

        if (rows.size() < 2)
        {
            continue;
        }

        double currHamming = 0.0;
        double hammingIncr = 1.0 / rows.size();

        for (std::size_t i = 0; i < rows.size(); i++)
        {
            currHamming += rows.at(i).hamming(rows.at((i + 1) % rows.size())) * hammingIncr;
        }

        bestKeys.push(std::make_pair(tryKeySize, currHamming));
    }

    std::vector<std::size_t> result;
    for (std::size_t i = 0; i < numberOfCandidatesToReturn; i++)
    {
        result.push_back(std::get<0>(bestKeys.top()));
        bestKeys.pop();
    }

    return result;
}

double DecryptorXor::measureConfidence(const ByteData &decipheredData) const
{
    ByteDistribution curDistribution(decipheredData);
    return curDistribution.distance(referenceLanguage_);
}

std::tuple<std::string, ByteData, double> DecryptorXor::decipherMultiKeySize(const ByteData &cipheredData,
                                                                             std::size_t keySize) const
{
    LOGIC_ASSERT(keySize >= 2);

    auto columns = cipheredData.extractColumns(keySize);
    ByteData key;

    for (auto const &bd : columns)
    {
        auto [ignore, keyPart, confidence] = decipherSingle(bd);
        key += keyPart;
    }

    auto decipheredStr = (cipheredData ^ key);
    auto confidence = measureConfidence(decipheredStr);
    return std::make_tuple(decipheredStr.str(ByteData::Encoding::plain), key, confidence);
}