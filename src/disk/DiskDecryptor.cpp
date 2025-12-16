#include "DiskDecryptor.h"
#include "BlockIO.h"

#include "../crypto/AES.h"
#include "../crypto/HybridEncryption.h"
#include "../recovery/ShamirSSS.h"
#include "../recovery/ShareStorage.h"

#include <iostream>
#include <vector>
#include <string>

void DiskDecryptor::decryptDisk(
    const std::string& inputFile,
    const std::string& outputFile,
    const std::vector<uint8_t>& pqcSecretKey,
    const std::string& sharesFolderPath,
    const std::vector<uint8_t>& masterKey
) {
    const size_t BLOCK_SIZE = 4096;

    // 1️Load Shamir shares and recover DEK
    std::vector<Share> shares;
    for (int i = 0; i < 5; ++i) { // Assuming 5 shares were created
        std::string sharePath = sharesFolderPath + "/share_" + std::to_string(i) + ".dat";
        shares.push_back(ShareStorage::loadShare(sharePath, masterKey));
    }

    std::vector<uint8_t> dek = ShamirSSS::recoverSecret(shares, 3); // Threshold = 3

    // 2️ Unwrap DEK using PQC private key (optional double-check)
    WrappedKey wrappedKey; // Assume you store/load wrappedKey metadata elsewhere
    dek = HybridEncryption::unwrapDEK(wrappedKey, pqcSecretKey);

    // 3️ Initialize BlockIO
    BlockIO blockIO(BLOCK_SIZE);

    if (!blockIO.openInput(inputFile)) {
        std::cerr << "[Error] Failed to open encrypted file: " << inputFile << "\n";
        return;
    }

    if (!blockIO.openOutput(outputFile)) {
        std::cerr << "[Error] Failed to open output file: " << outputFile << "\n";
        blockIO.closeFiles();
        return;
    }

    // 4️ Decrypt block-by-block
    std::vector<uint8_t> encryptedBlock;
    std::vector<uint8_t> plainBlock;

    while (blockIO.readBlock(encryptedBlock)) {
        plainBlock = AES::decrypt(encryptedBlock, dek);

        if (!blockIO.writeBlock(plainBlock)) {
            std::cerr << "[Error] Failed to write decrypted block\n";
            blockIO.closeFiles();
            return;
        }
    }

    // 5️ Close files
    blockIO.closeFiles();

    std::cout << "[Success] Disk decryption completed. Decrypted file: " << outputFile << "\n";
}
