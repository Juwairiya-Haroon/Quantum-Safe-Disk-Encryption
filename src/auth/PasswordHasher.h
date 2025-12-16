#pragma once
#include <string>

using namespace std;

class PasswordHasher {
public:
    static string hashPassword(const string& password);
    static bool verifyPassword(const string& password,
                               const string& storedHash);
};
