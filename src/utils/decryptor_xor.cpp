#include "decryptor_xor.h"
#include "byte_distribution.h"
#include <limits>
#include <tuple>

DecryptorXor::DecryptorXor(const std::string &referenceLanguageData)
    : referenceLanguage(ByteDistribution(ByteData(referenceLanguageData, ByteData::encoding::plain)))
{
}

std::tuple<std::string, std::byte, double> DecryptorXor::decipher(const ByteData &cipheredData) const
{
    auto candidate = std::make_tuple(cipheredData, std::byte{0}, std::numeric_limits<double>::max());
    unsigned char curKey = 0;
    do
    {
        auto curDecipher = cipheredData ^ std::byte{curKey};
        ByteDistribution curDistribution(curDecipher);
        auto curDistance = curDistribution.distance(referenceLanguage);

        if (curDistance < std::get<2>(candidate))
        {
            candidate = std::make_tuple(curDecipher, std::byte{curKey}, curDistance);
            // a little optimization for the trivial case, if we have exact match, no point in iterating further
            if (0.0 == curDistance)
            {
                break;
            }
        }

    } while (++curKey != 0);

    return std::make_tuple(std::get<0>(candidate).str(ByteData::encoding::plain), std::get<1>(candidate),
                           std::get<2>(candidate));
}