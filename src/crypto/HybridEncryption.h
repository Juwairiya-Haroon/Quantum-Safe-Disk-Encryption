#pragma once
#include <vector>
#include <cstdint>


struct WrappedKey {
    std::vector<uint8_t> pqcCiphertext;
    std::vector<uint8_t> wrappedDEK;
};

class HybridEncryption {
public:
    static std::vector<uint8_t> generateDEK();
    static WrappedKey wrapDEK(
        const std::vector<uint8_t>& dek,
        const std::vector<uint8_t>& pqcPublicKey);

    static std::vector<uint8_t> unwrapDEK(
        const WrappedKey& wrapped,
        const std::vector<uint8_t>& pqcSecretKey);
};
