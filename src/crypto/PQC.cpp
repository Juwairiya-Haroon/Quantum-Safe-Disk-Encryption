#include "PQC.h"
#include <cstdint>
#include <cstdlib>

static std::vector<uint8_t> randomBytes(size_t n) {
    std::vector<uint8_t> v(n);
    for (auto& b : v) b = rand() % 256;
    return v;
}

PQCKeyPair PQC::generateKeypair() {
    auto key = randomBytes(32);
    return { key, key };  // publicKey == secretKey
}


PQCResult PQC::encapsulate(const std::vector<uint8_t>& pk) {
    auto ss = randomBytes(32);
    auto ct = ss;
    for (size_t i = 0; i < ss.size(); i++)
        ct[i] ^= pk[i % pk.size()];
    return { ct, ss };
}

std::vector<uint8_t> PQC::decapsulate(
    const std::vector<uint8_t>& ct,
    const std::vector<uint8_t>& sk)
{
    std::vector<uint8_t> ss = ct;
    for (size_t i = 0; i < ct.size(); i++)
        ss[i] ^= sk[i % sk.size()];
    return ss;
}
