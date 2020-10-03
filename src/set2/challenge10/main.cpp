#include "aes.h"
#include "byte_data.h"
#include "file_utils.h"
#include "matasano_asserts.h"
#include <iostream>
#include <string>

static const std::string KEY = "YELLOW SUBMARINE";
static const std::vector<std::byte> IV(16, std::byte{0});

int main()
{
    auto cipher = ByteData(FileUtils::read("assets/10.txt", true), ByteData::Encoding::base64);
    ByteData key(KEY, ByteData::Encoding::plain);
    ByteData iv(IV);

    Aes aes(key, iv);

    auto deciphered = aes.decrypt(cipher);

    std::cout << "Deciphered text is:" << std::endl;
    std::cout << deciphered.str(ByteData::Encoding::plain) << std::endl;

    return 0;
}