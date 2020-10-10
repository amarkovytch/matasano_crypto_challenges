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

        ASSERT_TRUE(AesEcbOracle::isEcb(
            [&](const ByteData &plain) -> ByteData { return ecb.encrypt(prepend + plain + append); }));
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

        ASSERT_FALSE(AesEcbOracle::isEcb(
            [&](const ByteData &plain) -> ByteData { return ecb.encrypt(prepend + plain + append); }));
    }
}