#pragma once
#include <string>

using namespace std;

class PolicyEnforcer {
public:
    static bool canDecrypt(const string& username);
    static bool canRecoverKey(const string& username);
    static void registerFailedAttempt(const string& username);
};