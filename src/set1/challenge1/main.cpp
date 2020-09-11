#include "asserts.h"
#include "utils.h"
#include <iostream>
#include <string>

static std::string INPUT = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
static std::string EXP_OUTPUT = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";

int main()
{
    auto output = Utils::hexToBase64(INPUT);
    VALIDATE_EQ(output, EXP_OUTPUT);

    std::cout << "Success !" << std::endl;

    return 0;
}