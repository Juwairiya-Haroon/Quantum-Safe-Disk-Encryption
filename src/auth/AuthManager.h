#pragma once
#include <string>

using namespace std;

class AuthManager {
public:
    bool login(const string& username,
               const string& password,
               const string& otp);

    bool isAuthenticated() const;
    string getCurrentUser() const;

private:
    bool authenticated = false;
    string currentUser;
};