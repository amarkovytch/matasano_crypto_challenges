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
        auto str = FileUtils::read("assets/mobydick.txt");
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

    auto [retultStr, resultCipher, ignore] = trivial.decipher(trivialBD);
    ASSERT_EQ(trivialStr, retultStr);
    ASSERT_EQ(std::byte{0}, resultCipher);
}

TEST_F(DecryptorXorTest, AlmostTrivialCase)
{
    std::string trivialStr = "Almost Trivial test";
    std::byte cipher{33};

    ByteData triviallyCipheredBD = ByteData(trivialStr, ByteData::encoding::plain) ^ cipher;
    DecryptorXor trivial(trivialStr);

    auto [retultStr, resultCipher, ignore] = trivial.decipher(triviallyCipheredBD);
    ASSERT_EQ(trivialStr, retultStr);
    ASSERT_EQ(cipher, resultCipher);
}

TEST_F(DecryptorXorTest, DecryptEnglishText)
{
    std::string englishText = "My father works at the factory. My mother is a teacher."
                              "My sister goes to school. I go to school as well. I love to learn English very much !"
                              "English lessons in Soviet school were very good ! I Learned very good English !";

    std::byte cipher{89};
    ByteData cipherText = ByteData(englishText, ByteData::encoding::plain) ^ cipher;

    auto [retultStr, resultCipher, ignore] = decryptor->decipher(cipherText);
    ASSERT_EQ(englishText, retultStr);
    ASSERT_EQ(cipher, resultCipher);
}

TEST_F(DecryptorXorTest, CheckConfidence)
{
    std::string englishText = "My father works at the factory. My mother is a teacher."
                              "My sister goes to school. I go to school as well. I love to learn English very much !"
                              "English lessons in Soviet school were very good ! I Learned very good English !";

    std::string gibrishText =
        "a sdjfk ajsdfgi kaljsfhaskl;jdfh lsdjkghlskdjfghslkfjdglaksjdf;lwiru5 tow48uy5nh bvskjdfhgasdfg";

    std::byte cipher{89};
    ByteData cipherEnglishText = ByteData(englishText, ByteData::encoding::plain) ^ cipher;
    ByteData cipherGibrish = ByteData(gibrishText, ByteData::encoding::plain) ^ cipher;

    auto [retultStr, resultCipher, confidenceEnglish] = decryptor->decipher(cipherEnglishText);
    auto [ignore1, ignore2, confidenceGibrish] = decryptor->decipher(cipherGibrish);

    ASSERT_EQ(englishText, retultStr);
    ASSERT_EQ(cipher, resultCipher);

    ASSERT_LT(confidenceEnglish, confidenceGibrish);
}
