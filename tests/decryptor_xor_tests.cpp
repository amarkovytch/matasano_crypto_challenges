#include "byte_data.h"
#include "decryptor_xor.h"
#include "file_utils.h"
#include "gtest/gtest.h"
#include <memory>
#include <string>

class DecryptorXorTest : public ::testing::Test
{
protected:
    static void SetUpTestSuite()
    {
        auto str = FileUtils::readFile("assets/mobydick.txt");
        decryptor = std::make_shared<DecryptorXor>(str);
    }

    static std::shared_ptr<DecryptorXor> decryptor;
};

std::shared_ptr<DecryptorXor> DecryptorXorTest::decryptor = nullptr;

TEST_F(DecryptorXorTest, TrivialCase)
{
    std::string trivialStr = "Trivial test";
    ByteData trivialBD(trivialStr, ByteData::encoding::plain);
    DecryptorXor trivial(trivialStr);
    ASSERT_EQ(std::make_pair(trivialStr, std::byte{0}), trivial.decipher(trivialBD));
}

TEST_F(DecryptorXorTest, AlmostTrivialCase)
{
    std::string trivialStr = "Almost Trivial test";
    std::byte cipher{33};

    ByteData triviallyCipheredBD = ByteData(trivialStr, ByteData::encoding::plain) ^ cipher;
    DecryptorXor trivial(trivialStr);

    ASSERT_EQ(std::make_pair(trivialStr, cipher), trivial.decipher(triviallyCipheredBD));
}

TEST_F(DecryptorXorTest, DecryptEnglishText)
{
    std::string englishText = "My father works at the factory. My mother is a teacher."
                              "My sister goes to school. I go to school as well. I love to learn English very much !"
                              "English lessons in Soviet school were very good ! I Learned very good English !";

    std::byte cipher{89};
    ByteData cipherText = ByteData(englishText, ByteData::encoding::plain) ^ cipher;

    ASSERT_EQ(std::make_pair(englishText, cipher), decryptor->decipher(cipherText));
}
