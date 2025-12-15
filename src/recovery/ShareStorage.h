#pragma once
#include "ShamirSSS.h"
#include <string>

class ShareStorage {
public:
    static void storeShare(
    const Share& share,
    const std::string& path,
    const std::vector<uint8_t>& masterKey
);

    static Share loadShare(
        const std::string& path,
        const std::vector<uint8_t>& masterKey
    );
};
