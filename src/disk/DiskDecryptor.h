#pragma once
#include <string>
#include <vector>
#include <cstdint>

class DiskDecryptor {
public:
    /**
     * Decrypt a disk/file that was encrypted using DiskEncryptor.
     * @param inputFile Path to the encrypted file
     * @param outputFile Path to write the decrypted file
     * @param pqcSecretKey PQC private key to unwrap DEK
     * @param sharesFolderPath Path where Shamir shares are stored
     * @param masterKey Optional key used to decrypt shares
     */
    void decryptDisk(
        const std::string& inputFile,
        const std::string& outputFile,
        const std::vector<uint8_t>& pqcSecretKey,
        const std::string& sharesFolderPath,
        const std::vector<uint8_t>& masterKey
    );
};
