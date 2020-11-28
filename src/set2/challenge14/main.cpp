#include <functional>
#include <iostream>

#include "aes.h"
#include "aes_ecb_oracle.h"
#include "byte_data.h"
#include "general_utils.h"
#include "matasano_asserts.h"

static const std::string TARGET_BYTES = "This is a message I would like to decrypt in this cool exercise !";

static const auto KEY = GeneralUtils::randomData(16);
static const auto RANDOM_PREFIX = GeneralUtils::randomData(GeneralUtils::randomNum(0, 40));

static ByteData randomEncryptor(const ByteData &plain)
{
    Aes aes(KEY, ByteData(), Aes::Mode::ecb);
    return aes.encrypt(RANDOM_PREFIX + plain + ByteData(TARGET_BYTES, ByteData::Encoding::plain));
}

int main()
{
    AesEcbOracle oracle(randomEncryptor, AesEcbOracle::EncryptorType::ConstantRandom_Plain_Secret);
    auto recovered = oracle.recoverSecret();

    VALIDATE_STR_EQ(TARGET_BYTES, recovered.str(ByteData::Encoding::plain));
    std::cout << "Success" << std::endl;

    return 0;
}