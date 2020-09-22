#include "byte_data.h"
#include "decryptor_xor.h"
#include "file_utils.h"
#include "matasano_asserts.h"

#include <iostream>
#include <limits>
#include <string>

int main()
{
    auto cipheredStrings = FileUtils::readLines("assets/4.txt");
    auto referenceEnglish = FileUtils::read("assets/lotr.txt");
    DecryptorXor decryptor(referenceEnglish);

    std::string resultString = cipheredStrings.at(0);
    auto resultDecipher = std::make_tuple(std::string(), std::byte{0}, std::numeric_limits<double>::max());

    for (auto const &cipheredString : cipheredStrings)
    {
        auto candidateDecipher = decryptor.decipherSingle(cipheredString);
        if (std::get<2>(candidateDecipher) < std::get<2>(resultDecipher))
        {
            resultDecipher = candidateDecipher;
            resultString = cipheredString;
        }
    }

    auto [decipher, key, ignore] = resultDecipher;
    std::cout << "The ciphered string was: " << resultString << std::endl;
    std::cout << "The deciphered result is: " << decipher << std::endl;
    std::cout << "The key is: " << std::to_integer<unsigned int>(key) << std::endl;
}