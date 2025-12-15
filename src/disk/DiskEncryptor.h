#ifndef DISK_ENCRYPTOR_H
#define DISK_ENCRYPTOR_H

#include <string>
#include <vector>

/*
 * DiskEncryptor
 * --------------
 * Handles file-level encryption using:
 * - BlockIO for file I/O
 * - AES for data encryption
 * - HybridEncryption for DEK generation and protection
 */
class DiskEncryptor {
public:
    /*
     * Encrypts an input file and writes encrypted output.
     *
     * @param inputPath   Path of the plaintext input file
     * @param outputPath  Path of the encrypted output file
     * @param publicKey   PQC public key used to wrap the DEK
     * @return true if encryption succeeds
     */
    bool encryptFile(const std::string& inputPath,
                     const std::string& outputPath,
                     const std::vector<unsigned char>& publicKey);
};

#endif // DISK_ENCRYPTOR_H
