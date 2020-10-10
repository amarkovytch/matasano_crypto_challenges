#include "byte_data.h"
#include "matasano_asserts.h"
#include "padder.h"
#include <iostream>
#include <string>

static const std::string INPUT = "YELLOW SUBMARINE";
static const std::string EXP_OUTPUT = "YELLOW SUBMARINE\x04\x04\x04\x04";

int main()
{
    ByteData data(INPUT, ByteData::Encoding::plain);
    auto padded = Padder::pad(data, 4);

    VALIDATE_STR_EQ(EXP_OUTPUT, padded.str(ByteData::Encoding::plain));

    std::cout << "Success !" << std::endl;

    return 0;
}