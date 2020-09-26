#include "padder.h"
#include "byte_data.h"

ByteData Padder::pad(const ByteData &b, std::uint8_t bytesNumToPad)
{
    ByteData result = b;
    for (std::uint8_t i = 0; i < bytesNumToPad; i++)
    {
        result += std::byte{bytesNumToPad};
    }

    return result;
}