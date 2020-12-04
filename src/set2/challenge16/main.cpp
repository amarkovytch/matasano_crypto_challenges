#include <functional>
#include <iostream>

#include "aes.h"
#include "aes_cbc_manipulator.h"
#include "general_utils.h"
#include "matasano_asserts.h"

const std::string PRE = "comment1=cooking%20MCs;userdata=";
const std::string POST = ";comment2=%20like%20a%20pound%20of%20bacon";

const std::string DESIRED = ";admin=true;";

static bool decryptLookForAdmin(const Aes &aes, const ByteData &cipher)
{
    auto decrypted = aes.decrypt(cipher);
    return decrypted.str(ByteData::Encoding::plain).find(DESIRED) != std::string::npos;
}

int main()
{
    auto key = GeneralUtils::randomData(16);
    auto iv = GeneralUtils::randomData(16);

    Aes aes(key, iv);

    ByteData pre(PRE, ByteData::Encoding::plain);
    ByteData post(POST, ByteData::Encoding::plain);
    ByteData desired(DESIRED, ByteData::Encoding::plain);

    AesCbcManipulator::EncryptorConstPrePlainConstPost encryptor(pre, post, key, iv);
    AesCbcManipulator manipulator(encryptor);

    auto cipher = manipulator.manipulateNextBlockAfterPlain(desired);

    VALIDATE_EQ(true, decryptLookForAdmin(aes, cipher));
    std::cout << "Success" << std::endl;

    return 0;
}