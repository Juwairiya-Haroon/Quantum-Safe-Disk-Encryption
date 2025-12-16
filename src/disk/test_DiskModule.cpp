#include "DiskEncryptor.h"
#include "DiskDecryptor.h"
#include <iostream>
#include <vector>
#include <string>

int main() {
    std::string inputFile = "test_input.dat";
    std::string encryptedFile = "test_encrypted.dat";
    std::string decryptedFile = "test_decrypted.dat";
    std::string sharesFolder = "shares"; // Make sure this folder exists
    std::vector<uint8_t> masterKey = {0x01, 0x02, 0x03, 0x04}; // optional
    std::vector<uint8_t> pqcPublicKey = {0xAA, 0xBB, 0xCC};     // example key

    std::cout << "==== Disk Module Test ====" << std::endl;

    // Encrypt
    DiskEncryptor encryptor;
    encryptor.encryptDisk(inputFile, encryptedFile, pqcPublicKey, sharesFolder, masterKey);

    // Decrypt
    DiskDecryptor decryptor;
    decryptor.decryptDisk(encryptedFile, decryptedFile, pqcPublicKey, sharesFolder, masterKey);

    std::cout << "==== Test Completed ====" << std::endl;
    return 0;
}
