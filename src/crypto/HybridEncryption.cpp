#include "HybridEncryption.h"
#include "PQC.h"
#include "AES.h"
#include <cstdlib>

static std::vector<uint8_t> randomBytes(size_t n) {
    std::vector<uint8_t> v(n);
    for (auto& b : v) b = rand() % 256;
    return v;
}

std::vector<uint8_t> HybridEncryption::generateDEK() {
    return randomBytes(32); // AES-256 equivalent
}

WrappedKey HybridEncryption::wrapDEK(
    const std::vector<uint8_t>& dek,
    const std::vector<uint8_t>& pk)
{
    auto enc = PQC::encapsulate(pk);
    auto wrapped = AES::encrypt(dek, enc.sharedSecret);
    return { enc.ciphertext, wrapped };
}

std::vector<uint8_t> HybridEncryption::unwrapDEK(
    const WrappedKey& wrapped,
    const std::vector<uint8_t>& sk)
{
    auto ss = PQC::decapsulate(wrapped.pqcCiphertext, sk);
    return AES::decrypt(wrapped.wrappedDEK, ss);
}
