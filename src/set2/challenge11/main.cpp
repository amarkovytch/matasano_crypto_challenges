#include <functional>
#include <iostream>

#include "aes.h"
#include "aes_ecb_oracle.h"
#include "byte_data.h"
#include "general_utils.h"
#include "matasano_asserts.h"

static ByteData randomEncryptor(const ByteData &plain, bool &outIsEcb)
{
    auto key = GeneralUtils::randomData(16);
    auto iv = GeneralUtils::randomData(16);

    auto prependLength = GeneralUtils::randomNum(5, 10);
    auto appendLength = GeneralUtils::randomNum(5, 10);

    auto prepend = GeneralUtils::randomData(prependLength);
    auto append = GeneralUtils::randomData(appendLength);

    outIsEcb = (GeneralUtils::randomNum(0, 1) == 1) ? true : false;

    Aes aes(key, iv, outIsEcb ? Aes::Mode::ecb : Aes::Mode::cbc);
    return aes.encrypt(prepend + plain + append);
}

int main()
{
    using namespace std::placeholders;
    bool isEcb = false;

    for (std::size_t i = 0; i < 1000; i++)
    {
        AesEcbOracle oracle([&isEcb](const ByteData &plain) { return randomEncryptor(plain, isEcb); },
                            AesEcbOracle::EcryptorType::SecretUpToBlock1_Plain_SecretUpToBlock2);
        VALIDATE_EQ(isEcb, oracle.isEcb());
    }

    std::cout << "Success" << std::endl;

    return 0;
}