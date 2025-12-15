#include "DiskEncryptor.h"
#include "BlockIO.h"
#include "../crypto/AES.h"
#include "../crypto/HybridEncryption.h"

#include <fstream>
#include <iostream>

// Encrypt a file using AES + PQC-wrapped DEK
bool DiskEncryptor::encryptFile(const std::string& inputPath,
                                const std::string& outputPath,
                                const std::vector<uint8_t>& publicKey) {
    const size_t BLOCK_SIZE = 4096;

    // 1. Generate Data Encryption Key (DEK)
    // This key is used ONLY for AES file encryption
    std::vector<uint8_t> dek =
        HybridEncryption::generateDEK();

    // 2. Wrap DEK using PQC public key
    // This protects the DEK for secure storage
    WrappedKey wrappedKey =
        HybridEncryption::wrapDEK(dek, publicKey);

    // 3. Initialize block-level file I/O
    BlockIO blockIO(BLOCK_SIZE);

    if (!blockIO.openInput(inputPath)) {
        std::cerr << "Failed to open input file\n";
        return false;
    }

    if (!blockIO.openOutput(outputPath)) {
        std::cerr << "Failed to open output file\n";
        return false;
    }

    /*
     * 4. Write encryption metadata at file start
     * Metadata format:
     * [BLOCK_SIZE]
     * [PQC_CIPHERTEXT_SIZE][PQC_CIPHERTEXT_BYTES]
     * [WRAPPED_DEK_SIZE][WRAPPED_DEK_BYTES]
     */
    {
        std::ofstream metaOut(outputPath, std::ios::binary | std::ios::app);
        if (!metaOut.is_open()) {
            std::cerr << "Failed to write metadata\n";
            return false;
        }

        size_t pqcSize = wrappedKey.pqcCiphertext.size();
        size_t dekSize = wrappedKey.wrappedDEK.size();

        metaOut.write(reinterpret_cast<const char*>(&BLOCK_SIZE), sizeof(BLOCK_SIZE));

        metaOut.write(reinterpret_cast<const char*>(&pqcSize), sizeof(pqcSize));
        metaOut.write(reinterpret_cast<const char*>(wrappedKey.pqcCiphertext.data()), pqcSize);

        metaOut.write(reinterpret_cast<const char*>(&dekSize), sizeof(dekSize));
        metaOut.write(reinterpret_cast<const char*>(wrappedKey.wrappedDEK.data()), dekSize);

        metaOut.close();
    }

    // 5. Encrypt file block-by-block using AES
    std::vector<uint8_t> plainBlock;
    std::vector<uint8_t> encryptedBlock;

    while (blockIO.readBlock(plainBlock)) {
        // Encrypt block using AES with DEK
        encryptedBlock = AES::encrypt(plainBlock, dek);

        // Write encrypted block to output file
        if (!blockIO.writeBlock(encryptedBlock)) {
            std::cerr << "Failed to write encrypted block\n";
            blockIO.closeFiles();
            return false;
        }
    }

    // 6. Close files
    blockIO.closeFiles();

    return true;
}
