#include "byte_data.h"
#include "matasano_asserts.h"

#include <iostream>
#include <string>

static const std::string INPUT1 = "1c0111001f010100061a024b53535009181c";
static const std::string INPUT2 = "686974207468652062756c6c277320657965";
static const std::string EXP_OUTPUT = "746865206b696420646f6e277420706c6179";

int main()
{
    ByteData x(INPUT1);
    ByteData y(INPUT2);
    VALIDATE_EQ((x ^ y).str(), EXP_OUTPUT);

    std::cout << "Success !" << std::endl;

    return 0;
}