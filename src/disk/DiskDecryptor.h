#pragma once
#include <vector>
#include <string>
#include <cstdint>  // <--- Required for uint8_t

struct WrappedKey; // Forward declaration if needed

class DiskDecryptor {
public:
    /*
     * Decrypts an encrypted file and writes the original file.
     *
     * @param inputPath    Path of the encrypted input file
     * @param outputPath   Path of the decrypted output file
     * @param secretKey    PQC secret key used to unwrap the DEK
     * @return true if decryption succeeds
     */
    bool decryptFile(const std::string& inputPath,
                     const std::string& outputPath,
                     const std::vector<uint8_t>& secretKey);  // <--- uint8_t
};
