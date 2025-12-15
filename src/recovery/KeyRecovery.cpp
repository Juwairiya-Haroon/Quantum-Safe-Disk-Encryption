#include "KeyRecovery.h"
#include "ShareStorage.h"
#include <stdexcept>

std::vector<uint8_t> KeyRecovery::recoverKey(
    const std::vector<std::string>& sharePaths,
    int threshold,
    const std::vector<uint8_t>& masterKey
) {
    if (sharePaths.size() < threshold)
        throw std::runtime_error("Insufficient shares for recovery");

    std::vector<Share> shares;
    for (int i = 0; i < threshold; i++) {
        shares.push_back(
            ShareStorage::loadShare(sharePaths[i], masterKey)
        );
    }

    return ShamirSSS::recoverSecret(shares, threshold);
}
