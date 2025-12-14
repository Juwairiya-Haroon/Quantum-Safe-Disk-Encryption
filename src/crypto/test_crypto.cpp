#include <iostream>
#include "AES.h"
#include "PQC.h"
#include "HybridEncryption.h"

int main() {
    std::cout << "=== Crypto Module Test ===\n";

    // 1. Generate PQC keys
    auto kp = PQC::generateKeypair();

    // 2. Generate DEK
    auto dek = HybridEncryption::generateDEK();

    // 3. Wrap DEK using PQC
    auto wrapped = HybridEncryption::wrapDEK(dek, kp.publicKey);

    // 4. Unwrap DEK
    auto recoveredDEK = HybridEncryption::unwrapDEK(wrapped, kp.secretKey);

    // 5. Verify
    if (dek == recoveredDEK)
        std::cout << "[OK] DEK recovered successfully\n";
    else
        std::cout << "[FAIL] DEK mismatch\n";

    // 6. Test AES substitute
    std::vector<uint8_t> data = { 'H','E','L','L','O' };
    auto enc = AES::encrypt(data, dek);
    auto dec = AES::decrypt(enc, dek);

    if (data == dec)
        std::cout << "[OK] AES encryption/decryption works\n";
    else
        std::cout << "[FAIL] AES error\n";

    return 0;
}
