#include "AuthManager.h"
#include "PasswordHasher.h"
#include "TwoFactor.h"
#include "PolicyEnforcer.h"
#include "AuditLogger.h"

using namespace std;

static const string storedUser = "admin";
static const string storedHash =
    PasswordHasher::hashPassword("admin123");

bool AuthManager::login(const string& username,
                        const string& password,
                        const string& otp) {

    if (username != "admin") {
        AuditLogger::logEvent("Invalid username attempt: " + username);
        PolicyEnforcer::registerFailedAttempt(username);
        return false;
    }

    if (!PasswordHasher::verifyPassword(password, storedHash)) {
        AuditLogger::logEvent("Wrong password for user: " + username);
        PolicyEnforcer::registerFailedAttempt(username);
        return false;
    }

    string generatedOTP = TwoFactor::generateOTP();
    if (!TwoFactor::verifyOTP(otp, generatedOTP)) {
        AuditLogger::logEvent("OTP failure for user: " + username);
        PolicyEnforcer::registerFailedAttempt(username);
        return false;
    }

    authenticated = true;
    currentUser = username;

    AuditLogger::logEvent("User logged in successfully: " + username);
    return true;
}