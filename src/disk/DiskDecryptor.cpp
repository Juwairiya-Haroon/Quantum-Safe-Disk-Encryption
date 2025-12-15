#include "DiskDecryptor.h"
#include "BlockIO.h"
#include "../crypto/AES.h"
#include "../crypto/HybridEncryption.h"

#include <fstream>
#include <iostream>
#include <vector>

bool DiskDecryptor::decryptFile(const std::string& inputPath,
                                const std::string& outputPath,
                                const std::vector<uint8_t>& secretKey) {
    // Open encrypted file in binary mode to read metadata
    std::ifstream encFile(inputPath, std::ios::binary);
    if (!encFile.is_open()) {
        std::cerr << "Failed to open encrypted file\n";
        return false;
    }

    // 1️ Read metadata
    size_t blockSize;
    encFile.read(reinterpret_cast<char*>(&blockSize), sizeof(blockSize));

    size_t pqcSize;
    encFile.read(reinterpret_cast<char*>(&pqcSize), sizeof(pqcSize));
    std::vector<uint8_t> pqcCiphertext(pqcSize);
    encFile.read(reinterpret_cast<char*>(pqcCiphertext.data()), pqcSize);

    size_t wrappedDEKSize;
    encFile.read(reinterpret_cast<char*>(&wrappedDEKSize), sizeof(wrappedDEKSize));
    std::vector<uint8_t> wrappedDEK(wrappedDEKSize);
    encFile.read(reinterpret_cast<char*>(wrappedDEK.data()), wrappedDEKSize);

    encFile.close();

    // 2️ Reconstruct WrappedKey struct
    WrappedKey wrappedKey;
    wrappedKey.pqcCiphertext = pqcCiphertext;
    wrappedKey.wrappedDEK = wrappedDEK;

    // 3️ Unwrap DEK using PQC secret key
    std::vector<uint8_t> dek = HybridEncryption::unwrapDEK(wrappedKey, secretKey);

    // 4️ Initialize BlockIO for file I/O
    BlockIO blockIO(blockSize);

    if (!blockIO.openInput(inputPath)) {
        std::cerr << "Failed to open encrypted file for reading blocks\n";
        return false;
    }

    if (!blockIO.openOutput(outputPath)) {
        std::cerr << "Failed to open output file for writing decrypted data\n";
        return false;
    }

    // Skip metadata bytes before reading encrypted blocks
    std::ifstream skipFile(inputPath, std::ios::binary);
    skipFile.seekg(sizeof(blockSize) + sizeof(pqcSize) + pqcSize + sizeof(wrappedDEKSize) + wrappedDEKSize);
    skipFile.close();

    // 5️ Decrypt file block-by-block
    std::vector<uint8_t> encryptedBlock;
    std::vector<uint8_t> plainBlock;

    while (blockIO.readBlock(encryptedBlock)) {
        // AES decryption
        plainBlock = AES::decrypt(encryptedBlock, dek);

        // Write decrypted block
        if (!blockIO.writeBlock(plainBlock)) {
            std::cerr << "Failed to write decrypted block\n";
            blockIO.closeFiles();
            return false;
        }
    }

    // 6️ Close files
    blockIO.closeFiles();

    return true;
}
