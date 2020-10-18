#include <iostream>

#include "aes.h"
#include "byte_data.h"
#include "crypto_constants.h"
#include "general_utils.h"
#include "padder.h"

static const auto KEY = GeneralUtils::randomData(16);
static const Aes aes(KEY, ByteData(), Aes::Mode::ecb);

static std::string profile_for(const std::string &email)
{
    if (email.find_first_of("&=") != std::string::npos)
    {
        throw std::invalid_argument("Invalid email !");
    }
    return "email=" + email + "&uid=10&role=user";
}

int main()
{
    // step 1 let's see how just the word 'admin' looks encrypted
    // the trick is to make 'admin' to be the only data in the block
    const std::string EMAIL_EQ = "email=";

    // move 'admin' to the second block and pad it
    auto user = std::string(CryptoConstants::BLOCK_SIZE_BYTES - EMAIL_EQ.size(), 'a') +
                Padder::padToBlockSize(ByteData("admin", ByteData::Encoding::plain), CryptoConstants::BLOCK_SIZE_BYTES)
                    .str(ByteData::Encoding::plain);

    auto adminEncrypted = aes.encrypt(ByteData(profile_for(user), ByteData::Encoding::plain))
                              .extractRow(CryptoConstants::BLOCK_SIZE_BYTES, 1);

    // step 2 let's create user in such a way final block only has 'user' in it
    const std::string UID_ROLE = "&uid=10&role=";

    std::string userDomain = "@user.com";
    std::string username = std::string(
        CryptoConstants::BLOCK_SIZE_BYTES * 2 - (EMAIL_EQ.size() + userDomain.size() + UID_ROLE.size()), 'a');

    // user should now cause 'user' to appear in final 3d block
    user = username + userDomain;
    auto userEncrypted = aes.encrypt(ByteData(profile_for(user), ByteData::Encoding::plain));

    // step 3, replace the last part with the one we prepared earlier
    auto replacedAdmin = userEncrypted.subData(0, CryptoConstants::BLOCK_SIZE_BYTES * 2) + adminEncrypted;

    // finally, let's check we got admin
    const std::string expectedString = EMAIL_EQ + user + "&uid=10&role=admin";
    auto decryptedString = aes.decrypt(replacedAdmin).str(ByteData::Encoding::plain);
    VALIDATE_STR_EQ(expectedString, decryptedString);

    std::cout << "Success ! Gave admin to " + user + " user!" << std::endl;

    return 0;
}