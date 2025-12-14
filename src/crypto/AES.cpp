#include "AES.h"

std::vector<uint8_t> AES::encrypt(
    const std::vector<uint8_t>& data,
    const std::vector<uint8_t>& key)
{
    std::vector<uint8_t> out = data;
    for (size_t i = 0; i < data.size(); i++)
        out[i] ^= key[i % key.size()];
    return out;
}

std::vector<uint8_t> AES::decrypt(
    const std::vector<uint8_t>& data,
    const std::vector<uint8_t>& key)
{
    return encrypt(data, key); // XOR is reversible
}
