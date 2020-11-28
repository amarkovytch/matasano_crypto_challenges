#include <functional>
#include <iostream>

#include "byte_data.h"
#include "general_utils.h"
#include "matasano_asserts.h"
#include "padder.h"

int main()
{
    const std::string paddedStr = "ICE ICE BABY\x04\x04\x04\x04";
    const std::string unpaddedStr = "ICE ICE BABY";
    VALIDATE_STR_EQ(
        unpaddedStr,
        Padder::removePadding(ByteData(paddedStr, ByteData::Encoding::plain)).str(ByteData::Encoding::plain));

    const std::string invPaddedStr1 = "ICE ICE BABY\x05\x05\x05\x05";
    VALIDATE_EXCEPTION(Padder::removePadding(ByteData(invPaddedStr1, ByteData::Encoding::plain)),
                       std::invalid_argument);

    const std::string invPaddedStr2 = "ICE ICE BABY\x01\x02\x03\x04";
    VALIDATE_EXCEPTION(Padder::removePadding(ByteData(invPaddedStr2, ByteData::Encoding::plain)),
                       std::invalid_argument);

    std::cout << "Success" << std::endl;

    return 0;
}