#pragma once
#include <vector>
#include <cstdint>

struct PQCKeyPair {
    std::vector<uint8_t> publicKey;
    std::vector<uint8_t> secretKey;
};

struct PQCResult {
    std::vector<uint8_t> ciphertext;
    std::vector<uint8_t> sharedSecret;
};

class PQC {
public:
    static PQCKeyPair generateKeypair();
    static PQCResult encapsulate(const std::vector<uint8_t>& pk);
    static std::vector<uint8_t> decapsulate(
        const std::vector<uint8_t>& ct,
        const std::vector<uint8_t>& sk);
};
