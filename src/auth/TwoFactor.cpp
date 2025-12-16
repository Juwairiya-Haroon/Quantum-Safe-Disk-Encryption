#include "TwoFactor.h"

using namespace std;

string TwoFactor::generateOTP() {
    return "123456"; // simulated OTP
}

bool TwoFactor::verifyOTP(const string& inputOTP,
                          const string& actualOTP) {
    return inputOTP == actualOTP;
}