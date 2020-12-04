#include "aes.h"
#include "aes_cbc_manipulator.h"
#include "crypto_constants.h"
#include "general_utils.h"
#include "gtest/gtest.h"

static ByteData KEY = GeneralUtils::randomData(16);
static ByteData IV = GeneralUtils::randomData(16);

static Aes AES = Aes(KEY, IV);

TEST(CbcManipulatorTest, TestInvalidInput)
{
    ByteData pre("pre", ByteData::Encoding::plain);
    ByteData post("post", ByteData::Encoding::plain);
    ByteData desiredPost("01234567890123456789", ByteData::Encoding::plain);

    AesCbcManipulator::EncryptorConstPrePlainConstPost encryptor(pre, post, KEY, IV);
    AesCbcManipulator manipulator(encryptor);

    ASSERT_THROW(manipulator.manipulateNextBlockAfterPlain(desiredPost), std::invalid_argument);
}

TEST(CbcManipulatorTest, TestSameSize)
{
    ByteData pre("pre", ByteData::Encoding::plain);
    ByteData post("post", ByteData::Encoding::plain);
    ByteData desiredPost("past", ByteData::Encoding::plain);

    AesCbcManipulator::EncryptorConstPrePlainConstPost encryptor(pre, post, KEY, IV);
    AesCbcManipulator manipulator(encryptor);

    ByteData encrypted = manipulator.manipulateNextBlockAfterPlain(desiredPost);
    ByteData decryptedPost = AES.decrypt(encrypted).extractRow(CryptoConstants::BLOCK_SIZE_BYTES, 2);

    ASSERT_EQ(desiredPost, decryptedPost);
}

TEST(CbcManipulatorTest, TestNoSameSize)
{
    ByteData pre("pre", ByteData::Encoding::plain);
    ByteData post("xxx", ByteData::Encoding::plain);
    ByteData desiredPost("past", ByteData::Encoding::plain);

    AesCbcManipulator::EncryptorConstPrePlainConstPost encryptor(pre, post, KEY, IV);
    AesCbcManipulator manipulator(encryptor);

    ByteData encrypted = manipulator.manipulateNextBlockAfterPlain(desiredPost);
    ByteData decryptedPost = AES.decrypt(encrypted).extractRow(CryptoConstants::BLOCK_SIZE_BYTES, 2);

    ASSERT_EQ(desiredPost, decryptedPost);
}

TEST(CbcManipulatorTest, TestLongPost)
{
    ByteData pre("pre", ByteData::Encoding::plain);
    ByteData post("01234567890123456789", ByteData::Encoding::plain);
    ByteData desiredPost("past", ByteData::Encoding::plain);

    AesCbcManipulator::EncryptorConstPrePlainConstPost encryptor(pre, post, KEY, IV);
    AesCbcManipulator manipulator(encryptor);

    ByteData encrypted = manipulator.manipulateNextBlockAfterPlain(desiredPost);
    ByteData decryptedPost = AES.decrypt(encrypted).extractRow(CryptoConstants::BLOCK_SIZE_BYTES, 2);

    ASSERT_EQ(desiredPost, decryptedPost);
}

TEST(CbcManipulatorTest, TestLongPre)
{
    ByteData pre("abcdefghtabcdefghtabcdefghtabcdfffefght", ByteData::Encoding::plain);
    ByteData post("01234567890123456789", ByteData::Encoding::plain);
    ByteData desiredPost("past", ByteData::Encoding::plain);

    AesCbcManipulator::EncryptorConstPrePlainConstPost encryptor(pre, post, KEY, IV);
    AesCbcManipulator manipulator(encryptor);

    ByteData encrypted = manipulator.manipulateNextBlockAfterPlain(desiredPost);
    ByteData decryptedPost = AES.decrypt(encrypted).extractRow(CryptoConstants::BLOCK_SIZE_BYTES, 4);

    ASSERT_EQ(desiredPost, decryptedPost);
}

TEST(CbcManipulatorTest, TestPrePostBlockSize)
{
    ByteData pre("01234567890123450123456789012345", ByteData::Encoding::plain);
    ByteData post("abcdefgabcdefgrrabcdefgabcdefgrrabcdefgabcdefgrr", ByteData::Encoding::plain);
    ByteData desiredPost("past", ByteData::Encoding::plain);

    AesCbcManipulator::EncryptorConstPrePlainConstPost encryptor(pre, post, KEY, IV);
    AesCbcManipulator manipulator(encryptor);

    ByteData encrypted = manipulator.manipulateNextBlockAfterPlain(desiredPost);
    ByteData decryptedPost = AES.decrypt(encrypted).extractRow(CryptoConstants::BLOCK_SIZE_BYTES, 3);

    ASSERT_EQ(desiredPost, decryptedPost);
}