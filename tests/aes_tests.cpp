#include "aes.h"
#include "gtest/gtest.h"

TEST(AesTest, EncryptDecryptEcb)
{
    ByteData b1("This is a test data to encrypt", ByteData::Encoding::plain);
    ByteData key("0123456789abcdef", ByteData::Encoding::plain);

    Aes aes(key, ByteData(), Aes::Mode::ecb);

    auto encrypted = aes.encrypt(b1);
    auto decrypted = aes.decrypt(encrypted);

    ASSERT_EQ(b1, decrypted);
}

TEST(AesTest, EncryptDecryptCbcWrongIv)
{
    ByteData b1("This is a test data to encrypt", ByteData::Encoding::plain);
    ByteData key("0123456789abcdef", ByteData::Encoding::plain);

    ASSERT_THROW(Aes(key, ByteData(), Aes::Mode::cbc), std::invalid_argument);
}

TEST(AesTest, EncryptDecryptCbc)
{
    ByteData b1("This is a test data to encrypt", ByteData::Encoding::plain);
    ByteData key("0123456789abcdef", ByteData::Encoding::plain);
    ByteData iv("fedcba9876543210", ByteData::Encoding::plain);

    Aes aes(key, iv, Aes::Mode::cbc);

    auto encrypted = aes.encrypt(b1);
    auto decrypted = aes.decrypt(encrypted);

    ASSERT_EQ(b1, decrypted);
}