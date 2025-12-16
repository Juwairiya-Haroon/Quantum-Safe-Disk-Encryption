#pragma once
#include <string>
#include <vector>
#include <cstdint>

// Include ShamirSSS for Share type
#include "../recovery/ShamirSSS.h"
#include "../crypto/HybridEncryption.h"

class DiskEncryptor {
public:
    void encryptDisk(
        const std::string& inputFile,
        const std::string& outputFile,
        const std::vector<uint8_t>& pqcPublicKey,
        const std::string& sharesFolderPath,
        const std::vector<uint8_t>& masterKey
    );
};
