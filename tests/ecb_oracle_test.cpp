#include "aes.h"
#include "aes_ecb_oracle.h"
#include "byte_data.h"
#include "crypto_constants.h"
#include "general_utils.h"
#include "gtest/gtest.h"

TEST(EcbOracletTest, TestEcb)
{
    auto key = GeneralUtils::randomData(16);
    Aes ecb(key, ByteData(), Aes::Mode::ecb);

    for (std::size_t i; i <= CryptoConstants::BLOCK_SIZE_BYTES; i++)
    {
        auto prepend = GeneralUtils::randomData(GeneralUtils::randomNum(std::size_t{0}, i));
        auto append = GeneralUtils::randomData(GeneralUtils::randomNum(std::size_t{0}, i));
        AesEcbOracle oracle([&](const ByteData &plain) -> ByteData { return ecb.encrypt(prepend + plain + append); },
                            AesEcbOracle::EcryptorType::SecretUpToBlock1_Plain_SecretUpToBlock2);

        ASSERT_TRUE(oracle.isEcb());
    }
}

TEST(EcbOracletTest, TestNotEcb)
{
    auto key = GeneralUtils::randomData(16);
    auto iv = GeneralUtils::randomData(16);
    Aes ecb(key, iv, Aes::Mode::cbc);

    for (std::size_t i; i <= CryptoConstants::BLOCK_SIZE_BYTES; i++)
    {
        auto prepend = GeneralUtils::randomData(GeneralUtils::randomNum(std::size_t{0}, i));
        auto append = GeneralUtils::randomData(GeneralUtils::randomNum(std::size_t{0}, i));

        AesEcbOracle oracle([&](const ByteData &plain) -> ByteData { return ecb.encrypt(prepend + plain + append); },
                            AesEcbOracle::EcryptorType::SecretUpToBlock1_Plain_SecretUpToBlock2);

        ASSERT_FALSE(oracle.isEcb());
    }
}

TEST(EcbOracletTest, WrongEncryptorType)
{
    auto key = GeneralUtils::randomData(16);
    auto secret = GeneralUtils::randomData(40);

    Aes ecb(key, ByteData(), Aes::Mode::ecb);

    AesEcbOracle oracle([&](const ByteData &plain) -> ByteData { return ecb.encrypt(plain + secret); },
                        AesEcbOracle::EcryptorType::SecretUpToBlock1_Plain_SecretUpToBlock2);

    ASSERT_THROW(oracle.recoverSecret(), std::invalid_argument);
}

TEST(EcbOracletTest, TestRetrieveRandomSecretData)
{
    auto key = GeneralUtils::randomData(16);
    auto secret = GeneralUtils::randomData(40);

    Aes ecb(key, ByteData(), Aes::Mode::ecb);

    AesEcbOracle oracle([&](const ByteData &plain) -> ByteData { return ecb.encrypt(plain + secret); },
                        AesEcbOracle::EcryptorType::Plain_Secret);

    auto recovered = oracle.recoverSecret();
    ASSERT_EQ(secret, recovered);
}

TEST(EcbOracletTest, TestRetrieveRandomSecretDataBlockSize)
{
    auto key = GeneralUtils::randomData(16);
    auto secret = GeneralUtils::randomData(16 * 4);

    Aes ecb(key, ByteData(), Aes::Mode::ecb);

    AesEcbOracle oracle([&](const ByteData &plain) -> ByteData { return ecb.encrypt(plain + secret); },
                        AesEcbOracle::EcryptorType::Plain_Secret);

    auto recovered = oracle.recoverSecret();
    ASSERT_EQ(secret, recovered);
}

TEST(EcbOracletTest, TestRetrieveKnownSecretData)
{
    auto key = GeneralUtils::randomData(16);
    auto secret = ByteData("0123456789abcdefrtyu", ByteData::Encoding::plain);
    Aes ecb(key, ByteData(), Aes::Mode::ecb);

    AesEcbOracle oracle([&](const ByteData &plain) -> ByteData { return ecb.encrypt(plain + secret); },
                        AesEcbOracle::EcryptorType::Plain_Secret);

    auto recovered = oracle.recoverSecret();
    ASSERT_EQ(secret, recovered);
}