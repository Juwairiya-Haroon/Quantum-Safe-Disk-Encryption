#pragma once
#include <string>

using namespace std;

class TwoFactor {
public:
    static string generateOTP();
    static bool verifyOTP(const string& inputOTP,
                          const string& actualOTP);
};
