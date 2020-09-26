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

TEST_F(DecryptorXorTest, TrivialCaseSingleXor)
{
    std::string trivialStr = "Trivial test";
    ByteData trivialBD(trivialStr, ByteData::encoding::plain);
    DecryptorXor trivial(trivialStr);

    auto [retultStr, resultCipher, ignore] = trivial.decipherSingle(trivialBD);
    ASSERT_EQ(trivialStr, retultStr);
    ASSERT_EQ(std::byte{0}, resultCipher);
}

TEST_F(DecryptorXorTest, AlmostTrivialCaseSingleXor)
{
    std::string trivialStr = "Almost Trivial test";
    std::byte cipher{33};

    ByteData triviallyCipheredBD = ByteData(trivialStr, ByteData::encoding::plain) ^ cipher;
    DecryptorXor trivial(trivialStr);

    auto [retultStr, resultCipher, ignore] = trivial.decipherSingle(triviallyCipheredBD);
    ASSERT_EQ(trivialStr, retultStr);
    ASSERT_EQ(cipher, resultCipher);
}

TEST_F(DecryptorXorTest, DecryptEnglishTextSingleXor)
{
    std::string englishText = "My father works at the factory. My mother is a teacher."
                              "My sister goes to school. I go to school as well. I love to learn English very much !"
                              "English lessons in Soviet school were very good ! I Learned very good English !";

    std::byte cipher{89};
    ByteData cipherText = ByteData(englishText, ByteData::encoding::plain) ^ cipher;

    auto [retultStr, resultCipher, ignore] = decryptor->decipherSingle(cipherText);
    ASSERT_EQ(englishText, retultStr);
    ASSERT_EQ(cipher, resultCipher);
}

TEST_F(DecryptorXorTest, CheckConfidenceSingleXor)
{
    std::string englishText = "My father works at the factory. My mother is a teacher."
                              "My sister goes to school. I go to school as well. I love to learn English very much !"
                              "English lessons in Soviet school were very good ! I Learned very good English !";

    std::string gibrishText =
        "a sdjfk ajsdfgi kaljsfhaskl;jdfh lsdjkghlskdjfghslkfjdglaksjdf;lwiru5 tow48uy5nh bvskjdfhgasdfg";

    std::byte cipher{89};
    ByteData cipherEnglishText = ByteData(englishText, ByteData::encoding::plain) ^ cipher;
    ByteData cipherGibrish = ByteData(gibrishText, ByteData::encoding::plain) ^ cipher;

    auto [retultStr, resultCipher, confidenceEnglish] = decryptor->decipherSingle(cipherEnglishText);
    auto [ignore1, ignore2, confidenceGibrish] = decryptor->decipherSingle(cipherGibrish);

    ASSERT_EQ(englishText, retultStr);
    ASSERT_EQ(cipher, resultCipher);

    ASSERT_LT(confidenceEnglish, confidenceGibrish);
}

TEST_F(DecryptorXorTest, WrongRangeMultiXor)
{
    ASSERT_THROW(decryptor->decipherMulti(std::byte{0}, std::pair(0, 10)), std::invalid_argument);
    ASSERT_THROW(decryptor->decipherMulti(std::byte{0}, std::pair(5, 4)), std::invalid_argument);
}

TEST_F(DecryptorXorTest, AlmostTrivialCaseMultiXor)
{
    std::string trivialStr = "Almost Trivial test";
    ByteData cipher{"3334", ByteData::encoding::hex};

    ByteData triviallyCipheredBD = ByteData(trivialStr, ByteData::encoding::plain) ^ cipher;
    DecryptorXor trivial(trivialStr);

    auto [retultStr, resultCipher, ignore] = trivial.decipherMulti(triviallyCipheredBD, std::pair(2, 5));
    ASSERT_EQ(trivialStr, retultStr);
    ASSERT_EQ(cipher, resultCipher);
}

TEST_F(DecryptorXorTest, DecryptEnglishTextMultiXor)
{
    std::string englishText = "My father works at the factory. My mother is a teacher."
                              "My sister goes to school. I go to school as well. I love to learn English very much !"
                              "English lessons in Soviet school were very good ! I Learned very good English !";

    ByteData cipher{"333435", ByteData::encoding::hex};
    ByteData cipherText = ByteData(englishText, ByteData::encoding::plain) ^ cipher;

    auto [retultStr, resultCipher, ignore] = decryptor->decipherMulti(cipherText, std::pair(2, 20));
    ASSERT_EQ(englishText, retultStr);
    ASSERT_EQ(cipher, resultCipher);
}

TEST_F(DecryptorXorTest, DecryptEnglishTextMultiXorTestRanges)
{
    std::string englishText = "My father works at the factory. My mother is a teacher."
                              "My sister goes to school. I go to school as well. I love to learn English very much !"
                              "English lessons in Soviet school were very good ! I Learned very good English !";

    ByteData cipher{"333435", ByteData::encoding::hex};
    ByteData cipherText = ByteData(englishText, ByteData::encoding::plain) ^ cipher;

    auto [retultStr, resultCipher, ignore] = decryptor->decipherMulti(cipherText, std::pair(2, 10));
    ASSERT_EQ(englishText, retultStr);
    ASSERT_EQ(cipher, resultCipher);

    std::tie(retultStr, resultCipher, ignore) = decryptor->decipherMulti(cipherText, std::pair(2, 20));
    ASSERT_EQ(englishText, retultStr);
    ASSERT_EQ(cipher, resultCipher);

    std::tie(retultStr, resultCipher, ignore) = decryptor->decipherMulti(cipherText, std::pair(3, 30));
    ASSERT_EQ(englishText, retultStr);
    ASSERT_EQ(cipher, resultCipher);
}

TEST_F(DecryptorXorTest, DecryptEnglishTextMultiXorTestKeySizes)
{
    std::string englishText =
        "My father works at the factory. My mother is a teacher."
        "My sister goes to school. I go to school as well. I love to learn English very much !"
        "English lessons in Soviet school were very good ! I Learned very good English ! This code is going to turn out"
        " to be surprisingly useful later on.Breaking repeating - "
        "key XOR(\" Vigenere \") statistically is obviously an academic exercise,"
        "a \" Crypto 101 \" thing.But more people \" know how \" to break it than can actually break it,"
        "and a similar technique breaks something much more important.";

    ByteData cipher{"333435", ByteData::encoding::hex};
    ByteData cipherText = ByteData(englishText, ByteData::encoding::plain) ^ cipher;

    auto [retultStr, resultCipher, ignore] = decryptor->decipherMulti(cipherText, std::pair(2, 10));
    ASSERT_EQ(englishText, retultStr);
    ASSERT_TRUE(cipher.eqCyclically(resultCipher));

    cipher = ByteData{"33343536", ByteData::encoding::hex};
    cipherText = ByteData(englishText, ByteData::encoding::plain) ^ cipher;

    std::tie(retultStr, resultCipher, ignore) = decryptor->decipherMulti(cipherText, std::pair(2, 10));
    ASSERT_EQ(englishText, retultStr);
    ASSERT_TRUE(cipher.eqCyclically(resultCipher));

    cipher = ByteData{"1234567890", ByteData::encoding::plain};
    cipherText = ByteData(englishText, ByteData::encoding::plain) ^ cipher;

    std::tie(retultStr, resultCipher, ignore) = decryptor->decipherMulti(cipherText, std::pair(2, 10));
    ASSERT_EQ(englishText, retultStr);
    ASSERT_TRUE(cipher.eqCyclically(resultCipher));
}

TEST_F(DecryptorXorTest, CheckConfidenceMultiXor)
{
    std::string englishText = "My father works at the factory. My mother is a teacher."
                              "My sister goes to school. I go to school as well. I love to learn English very much !"
                              "English lessons in Soviet school were very good ! I Learned very good English !";

    std::string gibrishText =
        "a sdjfk ajsdfgi kaljsfhaskl;jdfh lsdjkghlskdjfghslkfjdglaksjdf;lwiru5 tow48uy5nh bvskjdfhgasdfg";

    ByteData cipher("89909192", ByteData::encoding::hex);
    ByteData cipherEnglishText = ByteData(englishText, ByteData::encoding::plain) ^ cipher;
    ByteData cipherGibrish = ByteData(gibrishText, ByteData::encoding::plain) ^ cipher;

    auto [retultStr, resultCipher, confidenceEnglish] = decryptor->decipherMulti(cipherEnglishText, std::pair(2, 10));
    auto [ignore1, ignore2, confidenceGibrish] = decryptor->decipherMulti(cipherGibrish, std::pair(2, 10));

    ASSERT_EQ(englishText, retultStr);
    ASSERT_TRUE(cipher.eqCyclically(resultCipher));

    ASSERT_LT(confidenceEnglish, confidenceGibrish);
}
