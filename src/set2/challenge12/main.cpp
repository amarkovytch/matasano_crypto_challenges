#include <functional>
#include <iostream>

#include "aes.h"
#include "aes_ecb_oracle.h"
#include "byte_data.h"
#include "general_utils.h"
#include "matasano_asserts.h"

static const std::string PLAIN_HEX =
    "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUa"
    "GUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";

static const auto KEY = GeneralUtils::randomData(16);

static ByteData randomEncryptor(const ByteData &plain)
{
    Aes aes(KEY, ByteData(), Aes::Mode::ecb);
    return aes.encrypt(plain + ByteData(PLAIN_HEX, ByteData::Encoding::base64));
}

int main()
{
    using namespace std::placeholders;

    AesEcbOracle oracle(randomEncryptor, AesEcbOracle::EcryptorType::Plain_Secret);
    auto recovered = oracle.recoverSecret();

    std::cout << "The secret is:" << std::endl;
    std::cout << recovered.str(ByteData::Encoding::plain) << std::endl;
    std::cout << "Success" << std::endl;

    return 0;
}