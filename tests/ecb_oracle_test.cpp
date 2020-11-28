#include "aes.h"
#include "aes_ecb_oracle.h"
#include "byte_data.h"
#include "crypto_constants.h"
#include "general_utils.h"
#include "gtest/gtest.h"

TEST(EcbOracletTest, TestEcb)
{
    auto key = GeneralUtils::randomData(16);
    Aes aes(key, ByteData(), Aes::Mode::ecb);

    for (std::size_t i; i <= CryptoConstants::BLOCK_SIZE_BYTES; i++)
    {
        AesEcbOracle oracle(
            [&](const ByteData &plain) -> ByteData {
                auto prepend = GeneralUtils::randomData(GeneralUtils::randomNum(std::size_t{0}, i));
                auto append = GeneralUtils::randomData(GeneralUtils::randomNum(std::size_t{0}, i));

                return aes.encrypt(prepend + plain + append);
            },
            AesEcbOracle::EncryptorType::RandomUpToBlock1_Plain_RandomUpToBlock2);

        ASSERT_TRUE(oracle.isEcb());
    }
}

TEST(EcbOracletTest, TestNotEcb)
{
    auto key = GeneralUtils::randomData(16);
    auto iv = GeneralUtils::randomData(16);
    Aes aes(key, iv, Aes::Mode::cbc);

    for (std::size_t i; i <= CryptoConstants::BLOCK_SIZE_BYTES; i++)
    {
        AesEcbOracle oracle(
            [&](const ByteData &plain) -> ByteData {
                auto prepend = GeneralUtils::randomData(GeneralUtils::randomNum(std::size_t{0}, i));
                auto append = GeneralUtils::randomData(GeneralUtils::randomNum(std::size_t{0}, i));

                return aes.encrypt(prepend + plain + append);
            },
            AesEcbOracle::EncryptorType::RandomUpToBlock1_Plain_RandomUpToBlock2);

        ASSERT_FALSE(oracle.isEcb());
    }
}

TEST(EcbOracletTest, WrongEncryptorType)
{
    auto key = GeneralUtils::randomData(16);
    auto secret = GeneralUtils::randomData(40);

    Aes ecb(key, ByteData(), Aes::Mode::ecb);

    AesEcbOracle oracle([&](const ByteData &plain) -> ByteData { return ecb.encrypt(plain + secret); },
                        AesEcbOracle::EncryptorType::RandomUpToBlock1_Plain_RandomUpToBlock2);

    ASSERT_THROW(oracle.recoverSecret(), std::invalid_argument);
}

TEST(EcbOracletTest, TestRetrieveRandomSecretData)
{
    auto key = GeneralUtils::randomData(16);
    auto secret = GeneralUtils::randomData(40);

    Aes ecb(key, ByteData(), Aes::Mode::ecb);

    AesEcbOracle oracle([&](const ByteData &plain) -> ByteData { return ecb.encrypt(plain + secret); },
                        AesEcbOracle::EncryptorType::Plain_Secret);

    auto recovered = oracle.recoverSecret();
    ASSERT_EQ(secret, recovered);
}

TEST(EcbOracletTest, TestRetrieveRandomSecretDataBlockSize)
{
    auto key = GeneralUtils::randomData(16);
    auto secret = GeneralUtils::randomData(16 * 4);

    Aes ecb(key, ByteData(), Aes::Mode::ecb);

    AesEcbOracle oracle([&](const ByteData &plain) -> ByteData { return ecb.encrypt(plain + secret); },
                        AesEcbOracle::EncryptorType::Plain_Secret);

    auto recovered = oracle.recoverSecret();
    ASSERT_EQ(secret, recovered);
}

TEST(EcbOracletTest, TestRetrieveKnownSecretData)
{
    auto key = GeneralUtils::randomData(16);
    auto secret = ByteData("0123456789abcdefrtyu", ByteData::Encoding::plain);
    Aes ecb(key, ByteData(), Aes::Mode::ecb);

    AesEcbOracle oracle([&](const ByteData &plain) -> ByteData { return ecb.encrypt(plain + secret); },
                        AesEcbOracle::EncryptorType::Plain_Secret);

    auto recovered = oracle.recoverSecret();
    ASSERT_EQ(secret, recovered);
}

TEST(EcbOracletTest, TestRetrieveRandomSecretDataConstantRandom_Plain_Secret)
{
    auto key = GeneralUtils::randomData(16);
    auto random = GeneralUtils::randomData(GeneralUtils::randomNum(0, 100));
    auto secret = GeneralUtils::randomData(40);

    Aes ecb(key, ByteData(), Aes::Mode::ecb);

    AesEcbOracle oracle([&](const ByteData &plain) -> ByteData { return ecb.encrypt(random + plain + secret); },
                        AesEcbOracle::EncryptorType::ConstantRandom_Plain_Secret);

    auto recovered = oracle.recoverSecret();
    ASSERT_EQ(secret, recovered);
}

TEST(EcbOracletTest, TestRetrieveRandomSecretDataBlockSizeConstantRandom_Plain_Secret)
{
    auto key = GeneralUtils::randomData(16);
    auto random = GeneralUtils::randomData(32);
    auto secret = GeneralUtils::randomData(16 * 4);

    Aes ecb(key, ByteData(), Aes::Mode::ecb);

    AesEcbOracle oracle([&](const ByteData &plain) -> ByteData { return ecb.encrypt(random + plain + secret); },
                        AesEcbOracle::EncryptorType::ConstantRandom_Plain_Secret);

    auto recovered = oracle.recoverSecret();
    ASSERT_EQ(secret, recovered);
}

TEST(EcbOracletTest, TestRetrieveKnownSecretDataConstantRandom_Plain_Secret1)
{
    auto key = GeneralUtils::randomData(16);
    auto random = ByteData("123456789012345", ByteData::Encoding::plain);
    auto secret = ByteData("1123456789abcdefrtyu", ByteData::Encoding::plain);
    Aes ecb(key, ByteData(), Aes::Mode::ecb);

    AesEcbOracle oracle([&](const ByteData &plain) -> ByteData { return ecb.encrypt(random + plain + secret); },
                        AesEcbOracle::EncryptorType::ConstantRandom_Plain_Secret);

    auto recovered = oracle.recoverSecret();
    ASSERT_EQ(secret, recovered);
}

TEST(EcbOracletTest, TestRetrieveKnownSecretDataConstantRandom_Plain_Secret2)
{
    auto key = GeneralUtils::randomData(16);
    auto random = ByteData("1234567890123456123", ByteData::Encoding::plain);
    auto secret = ByteData("0123456789abcdefrtyu", ByteData::Encoding::plain);
    Aes ecb(key, ByteData(), Aes::Mode::ecb);

    AesEcbOracle oracle([&](const ByteData &plain) -> ByteData { return ecb.encrypt(random + plain + secret); },
                        AesEcbOracle::EncryptorType::ConstantRandom_Plain_Secret);

    auto recovered = oracle.recoverSecret();
    ASSERT_EQ(secret, recovered);
}