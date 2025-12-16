#include "PasswordHasher.h"
#include <functional>

using namespace std;

string PasswordHasher::hashPassword(const string& password) {
    hash<string> hasher;
    return to_string(hasher(password));
}

bool PasswordHasher::verifyPassword(const string& password,
                                    const string& storedHash) {
    return hashPassword(password) == storedHash;
}
