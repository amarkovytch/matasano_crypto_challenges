#include <algorithm>

#include "byte_data.h"
#include "general_utils.h"
#include "matasano_asserts.h"

ByteData GeneralUtils::randomData(std::size_t length)
{
    ByteData res(length);

    std::generate(res.secureData().begin(), res.secureData().end(),
                  []() { return GeneralUtils::randomNum<std::uint8_t>(0, std::numeric_limits<std::uint8_t>::max()); });

    return res;
}