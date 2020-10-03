#include "byte_data.h"
#include "decryptor_xor.h"
#include "file_utils.h"
#include "matasano_asserts.h"

#include <iostream>
#include <limits>
#include <string>

int main()
{
    auto cipheredBase64Str = FileUtils::read("assets/6.txt", true);
    auto referenceEnglish = FileUtils::read("assets/lotr.txt");
    DecryptorXor decryptor(referenceEnglish);

    ByteData cipheredBase64(cipheredBase64Str, ByteData::Encoding::base64);

    auto [decipher, key, ignore] = decryptor.decipherMulti(cipheredBase64, std::pair(2, 40));

    std::cout << "The deciphered result is: " << std::endl << decipher << std::endl;
    std::cout << "The key is: " << key.str(ByteData::Encoding::plain) << std::endl;

    return 0;
}