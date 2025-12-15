#pragma once
#include <vector>
#include <string>
#include <cstdint>

class KeyRecovery {
public:
    static std::vector<uint8_t> recoverKey(
        const std::vector<std::string>& sharePaths,
        int threshold,
        const std::vector<uint8_t>& masterKey
    );
};
