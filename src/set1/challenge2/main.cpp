#include "matasano_asserts.h"
#include "matasano_math.h"
#include "math.h"

#include <iostream>
#include <string>

static std::string INPUT1 = "1c0111001f010100061a024b53535009181c";
static std::string INPUT2 = "686974207468652062756c6c277320657965";
static std::string EXP_OUTPUT = "746865206b696420646f6e277420706c6179";

int main()
{
    auto output = Math::xorHexStrs(INPUT1, INPUT2);
    VALIDATE_EQ(output, EXP_OUTPUT);

    std::cout << "Success !" << std::endl;

    return 0;
}