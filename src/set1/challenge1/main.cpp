#include "byte_data.h"
#include "matasano_asserts.h"
#include <iostream>
#include <string>

static const std::string INPUT =
    "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
static const std::string EXP_OUTPUT = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";

int main()
{
    ByteData data(INPUT);
    VALIDATE_STR_EQ(data.str(ByteData::Encoding::base64), EXP_OUTPUT);

    std::cout << "Success !" << std::endl;

    return 0;
}