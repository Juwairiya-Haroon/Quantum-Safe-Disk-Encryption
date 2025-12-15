#pragma once
#include <vector>
#include <cstdint>

struct Share {
    uint8_t index;
    std::vector<uint8_t> value;
};

class ShamirSSS {
public:
    static std::vector<Share> splitSecret(
        const std::vector<uint8_t>& secret,
        int k,
        int n
    );

    static std::vector<uint8_t> recoverSecret(
        const std::vector<Share>& shares,
        int k
    );
};
