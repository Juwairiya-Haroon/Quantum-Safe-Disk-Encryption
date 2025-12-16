#include "DiskEncryptor.h"
#include "BlockIO.h"

#include "../crypto/AES.h"
#include "../crypto/HybridEncryption.h"
#include "../recovery/ShamirSSS.h"
#include "../recovery/ShareStorage.h"

#include <iostream>
#include <cstdint>
#include <string>
#include <vector>

void DiskEncryptor::encryptDisk(
    const std::string& inputFile,
    const std::string& outputFile,
    const std::vector<uint8_t>& pqcPublicKey,
    const std::string& sharesFolderPath,       // Path to store Shamir shares
    const std::vector<uint8_t>& masterKey      // Optional key to encrypt shares
) {
    const size_t BLOCK_SIZE = 4096;

    // 1️⃣ Generate Data Encryption Key (DEK)
    std::vector<uint8_t> dek = HybridEncryption::generateDEK();

    // 2️⃣ Split DEK into shares (threshold k=3, total n=5)
    std::vector<Share> shares = ShamirSSS::splitSecret(dek, 3, 5);

    // 3️⃣ Store shares securely
    for (size_t i = 0; i < shares.size(); ++i) {
        std::string sharePath = sharesFolderPath + "/share_" + std::to_string(i) + ".dat";
        ShareStorage::storeShare(shares[i], sharePath, masterKey);
    }

    // 4️⃣ Wrap DEK using PQC (Kyber-style or whatever PQC module)
    WrappedKey wrappedKey = HybridEncryption::wrapDEK(dek, pqcPublicKey);

    // 5️⃣ Initialize block I/O
    BlockIO blockIO(BLOCK_SIZE);

    if (!blockIO.openInput(inputFile)) {
        std::cerr << "[Error] Failed to open input file: " << inputFile << "\n";
        return;
    }

    if (!blockIO.openOutput(outputFile)) {
        std::cerr << "[Error] Failed to open output file: " << outputFile << "\n";
        blockIO.closeFiles();
        return;
    }

    // 6️⃣ Encrypt block-by-block
    std::vector<uint8_t> plainBlock;
    std::vector<uint8_t> encryptedBlock;

    while (blockIO.readBlock(plainBlock)) {
        // Encrypt using AES with DEK
        encryptedBlock = AES::encrypt(plainBlock, dek);

        // Write encrypted block
        if (!blockIO.writeBlock(encryptedBlock)) {
            std::cerr << "[Error] Failed to write encrypted block\n";
            blockIO.closeFiles();
            return;
        }
    }

    // 7️⃣ Close files
    blockIO.closeFiles();

    std::cout << "[Success] Disk encryption completed. Encrypted file: " << outputFile << "\n";
}
