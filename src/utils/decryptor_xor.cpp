#include "decryptor_xor.h"
#include "byte_distribution.h"
#include <limits>
#include <tuple>

DecryptorXor::DecryptorXor(const std::string &referenceLanguageData)
    : referenceLanguage(ByteDistribution(ByteData(referenceLanguageData, ByteData::encoding::plain)))
{
}

std::pair<std::string, std::byte> DecryptorXor::decipher(const ByteData &cipheredText) const
{
    auto candidate = std::make_tuple(std::numeric_limits<double>::max(), cipheredText, std::byte{0});
    unsigned char curKey = 0;
    do
    {
        auto curDecipher = cipheredText ^ std::byte{curKey};
        ByteDistribution curDistribution(curDecipher);
        auto curDistance = curDistribution.distance(referenceLanguage);

        if (curDistance < std::get<0>(candidate))
        {
            candidate = std::make_tuple(curDistance, curDecipher, std::byte{curKey});
            // a little optimization for the trivial case, if we have exact match, no point in iterating further
            if (0.0 == curDistance)
            {
                break;
            }
        }

    } while (++curKey != 0);

    return std::make_pair(std::get<1>(candidate).str(ByteData::encoding::plain), std::get<2>(candidate));
}