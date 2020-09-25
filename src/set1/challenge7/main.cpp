#include "byte_data.h"
#include "decryptor_xor.h"
#include "file_utils.h"
#include "matasano_asserts.h"

#include <cryptopp/aes.h>
#include <cryptopp/base64.h>
#include <cryptopp/files.h>
#include <cryptopp/modes.h>
#include <iostream>
#include <limits>
#include <string>

static const std::string KEY = "YELLOW SUBMARINE";

int main()
{
    std::vector<CryptoPP::byte> key;
    CryptoPP::StringSource(KEY, true, new CryptoPP::VectorSink(key));

    std::string ciphered;
    CryptoPP::FileSource("assets/7.txt", true, new CryptoPP::Base64Decoder(new CryptoPP::StringSink(ciphered)));

    CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption dec;
    dec.SetKey(key.data(), key.size());

    std::string deciphered;
    CryptoPP::StringSource(ciphered, true,
                           new CryptoPP::StreamTransformationFilter(dec, new CryptoPP::StringSink(deciphered)));

    std::cout << "The deciphered result is: " << std::endl << deciphered << std::endl;
}