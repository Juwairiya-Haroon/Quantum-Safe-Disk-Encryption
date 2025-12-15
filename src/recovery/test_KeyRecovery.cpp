#include <iostream>
#include <vector>
#include <cassert>
#include <sys/stat.h>
#include <sys/types.h>
#ifdef _WIN32
#include <direct.h>
#endif
#include "ShamirSSS.h"
#include "ShareStorage.h"
#include "KeyRecovery.h"

using namespace std;

int main() {
    cout << "[TEST] Key Recovery System\n";

    // -----------------------------
    // Step 1: Create AES Disk Key
    // -----------------------------
    vector<uint8_t> aesKey(32);
    for (int i = 0; i < 32; i++)
        aesKey[i] = static_cast<uint8_t>(i + 1);

    // Master key used to encrypt shares (password-derived in real system)
    vector<uint8_t> masterKey(32, 0xAA);

    int threshold = 3;
    int totalShares = 5;

    // -----------------------------
    // Step 2: Split Secret
    // -----------------------------
    auto shares = ShamirSSS::splitSecret(aesKey, threshold, totalShares);
    assert(shares.size() == totalShares);

    cout << " Secret split into shares\n";

    // -----------------------------
    // Step 3: Store Encrypted Shares
    // -----------------------------
    vector<string> sharePaths;

#ifdef _WIN32
    _mkdir("test_shares");
#else
    mkdir("test_shares", 0777);
#endif

    for (int i = 0; i < totalShares; i++) {
        string path = "test_shares/share_" + to_string(i) + ".bin";
        ShareStorage::storeShare(shares[i], path, masterKey);
        sharePaths.push_back(path);
    }

    cout << "Shares encrypted & stored\n";

    // -----------------------------
    // Step 4: Recover AES Key
    // -----------------------------
    auto recoveredKey =
        KeyRecovery::recoverKey(sharePaths, threshold, masterKey);

    assert(recoveredKey == aesKey);
    cout << "Key recovered successfully\n";

    // -----------------------------
    // Step 5: Failure Test
    // -----------------------------
    try {
        vector<string> insufficientShares = {
            sharePaths[0], sharePaths[1]
        };

        KeyRecovery::recoverKey(insufficientShares, threshold, masterKey);
        assert(false);  // Should NOT reach here
    }
    catch (const exception&) {
        cout << "Recovery failed correctly with insufficient shares\n";
    }

    // -----------------------------
    // Cleanup
    // -----------------------------
for (int i = 0; i < totalShares; i++) {
    remove(sharePaths[i].c_str());
}
rmdir("test_shares");

    cout << "[PASS] All KeyRecovery tests passed\n";
    return 0;
}
