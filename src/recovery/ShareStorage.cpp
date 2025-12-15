#include "ShareStorage.h"
#include "../crypto/AES.h"   
#include <fstream>

void ShareStorage::storeShare(
    const Share& share,
    const std::string& path,
    const std::vector<uint8_t>& masterKey
) {
    std::vector<uint8_t> data;
    data.push_back(share.index);
    data.insert(data.end(), share.value.begin(), share.value.end());

    // Encrypt share using Member-1 AES
    auto encrypted = AES::encrypt(data, masterKey);

    std::ofstream out(path, std::ios::binary);
    out.write((char*)encrypted.data(), encrypted.size());
}
Share ShareStorage::loadShare(
    const std::string& path,
    const std::vector<uint8_t>& masterKey
) {
    std::ifstream in(path, std::ios::binary);

    std::vector<uint8_t> encrypted(
        std::istreambuf_iterator<char>(in),
        {}
    );

    auto decrypted = AES::decrypt(encrypted, masterKey);

    Share s;
    s.index = decrypted[0];
    s.value.assign(decrypted.begin() + 1, decrypted.end());
    return s;
}

