#pragma once
#include <vector>
#include <cstdint>

class AES {
public:
    static std::vector<uint8_t> encrypt(
        const std::vector<uint8_t>& data,
        const std::vector<uint8_t>& key);

    static std::vector<uint8_t> decrypt(
        const std::vector<uint8_t>& data,
        const std::vector<uint8_t>& key);
};
