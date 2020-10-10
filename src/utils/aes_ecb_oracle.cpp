#include "aes_ecb_oracle.h"
#include "byte_data.h"
#include "crypto_constants.h"
#include <functional>

bool AesEcbOracle::isEcb(ecryptorFunction encryptor)
{
    ByteData identicalData3blocks(std::vector(CryptoConstants::BLOCK_SIZE_BYTES * 3, std::uint8_t{1}));
    auto result = encryptor(identicalData3blocks);

    // if this is ecb - the encrypted second and third blocks should be identical
    auto blocks = result.extractRows(CryptoConstants::BLOCK_SIZE_BYTES);

    return blocks.at(1) == blocks.at(2);
}