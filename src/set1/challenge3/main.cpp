#include "byte_data.h"
#include "decryptor_xor.h"
#include "file_utils.h"
#include "matasano_asserts.h"

#include <iostream>
#include <string>

static const std::string CIPHERED_TEXT = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";

int main()
{
    auto referenceEnglish = FileUtils::read("./assets/lotr.txt");
    DecryptorXor decryptor(referenceEnglish);
    auto [resultStr, resultCipher, ignore] = decryptor.decipherSingle(CIPHERED_TEXT);
    std::cout << "The text is: " << resultStr << std::endl;
    std::cout << "The cipher byte was: " << std::to_integer<unsigned int>(resultCipher) << std::endl;
}