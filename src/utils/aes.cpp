
#include <cryptopp/aes.h>
#include <cryptopp/base64.h>
#include <cryptopp/files.h>
#include <cryptopp/modes.h>

#include "aes.h"
#include "crypto_block_aggregator.h"
#include "crypto_constants.h"
#include "matasano_asserts.h"
#include "padder.h"

Aes::Aes(const ByteData &key, const ByteData &iv, Mode mode, KeySize keySize)
    : key_(key), iv_(iv), mode_(mode), keySize_(keySize)
{
    switch (mode_)
    {
    case (Aes::Mode::ecb):
        break;
    case (Aes::Mode::cbc):
        THROW_IF(iv.size() != 16, "iv should be 16 bytes long for cbc", std::invalid_argument);
        break;
    default:
        throw std::invalid_argument("Invalid mode");
    }

    switch (keySize_)
    {
    case (Aes::KeySize::bit128):
        THROW_IF(key.size() != 16, "key size should be 16 bytes", std::invalid_argument);
        break;
    default:
        throw std::invalid_argument("Invalid Key Size");
    }
}

ByteData Aes::encrypt(const ByteData &plain) const { return encryptDecrypt(plain, true); }

ByteData Aes::decrypt(const ByteData &plain) const { return encryptDecrypt(plain, false); }

ByteData Aes::encryptDecrypt(const ByteData &data, bool encrypt) const
{
    THROW_IF(data.size() == 0, "can't " + std::string(encrypt ? "encrypt" : "decrypt") + " empty data",
             std::invalid_argument);

    switch (mode_)
    {
    case (Aes::Mode::ecb):
        return encryptDecryptEcb(data, encrypt);
    case (Aes::Mode::cbc):
        return encryptDecryptCbc(data, encrypt);
    default:
        LOGIC_ASSERT(true);
    }

    LOGIC_ASSERT(true);
    return ByteData();
}

ByteData Aes::ecbEncryptDecryptBlock(const ByteData &block, bool encrypt) const
{
    LOGIC_ASSERT(block.size() % CryptoConstants::BLOCK_SIZE_BYTES == 0);
    ByteData result(block.size());

    CryptoPP::SecByteBlock key(key_.secureData().data(), key_.size());
    if (encrypt)
    {
        CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption ecbEncryption(key, key.size());
        ecbEncryption.ProcessData(result.secureData().data(), block.secureData().data(), result.size());
    }
    else
    {
        CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption ecbDecryption(key, key.size());
        ecbDecryption.ProcessData(result.secureData().data(), block.secureData().data(), result.size());
    }

    return result;
}

ByteData Aes::encryptDecryptEcb(const ByteData &data, bool encrypt) const
{
    LOGIC_ASSERT(data.size() != 0);

    CryptoBlockAggregator aggregator(data, encrypt ? CryptoBlockAggregator::Padding::PadOnGetBlock
                                                   : CryptoBlockAggregator::Padding::UnpadOnAggregateBlock);

    for (auto const &block : aggregator.blocksFromSource())
    {
        aggregator.aggregateBlock(ecbEncryptDecryptBlock(block, encrypt));
    }

    return aggregator.output();
}

ByteData Aes::encryptDecryptCbc(const ByteData &data, bool encrypt) const
{
    LOGIC_ASSERT(data.size() != 0);

    CryptoBlockAggregator aggregator(data, encrypt ? CryptoBlockAggregator::Padding::PadOnGetBlock
                                                   : CryptoBlockAggregator::Padding::UnpadOnAggregateBlock);

    auto prevCipheredBlock = iv_;

    for (auto const &block : aggregator.blocksFromSource())
    {
        if (encrypt)
        {
            prevCipheredBlock = ecbEncryptDecryptBlock(prevCipheredBlock ^ block, encrypt);
            aggregator.aggregateBlock(prevCipheredBlock);
        }
        else
        {
            aggregator.aggregateBlock(prevCipheredBlock ^ ecbEncryptDecryptBlock(block, encrypt));
            prevCipheredBlock = block;
        }
    }

    return aggregator.output();
}