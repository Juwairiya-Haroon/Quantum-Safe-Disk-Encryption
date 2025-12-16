#include <iostream>
#include <string>

#include "../auth/AuthManager.h"
#include "../auth/PolicyEnforcer.h"

using namespace std;

int main() {
    AuthManager auth;

    string username, password, otp;

    cout << "Username: ";
    cin >> username;

    cout << "Password: ";
    cin >> password;

    cout << "OTP: ";
    cin >> otp;

    if (!auth.login(username, password, otp)) {
        cout << "Authentication failed!" << endl;
        return 0;
    }

    cout << "Authentication successful!" << endl;

    if (PolicyEnforcer::canDecrypt(username)) {
        cout << "Authorization granted: You can decrypt disk." << endl;
    } else {
        cout << "Authorization denied: Too many failed attempts." << endl;
    }

    if (PolicyEnforcer::canRecoverKey(username)) {
        cout << "Authorization granted: You can recover key." << endl;
    } else {
        cout << "Authorization denied: Recovery locked." << endl;
    }

    return 0;
}
