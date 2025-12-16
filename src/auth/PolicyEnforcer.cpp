#include "PolicyEnforcer.h"
#include <unordered_map>
#include "AuditLogger.h"

using namespace std;

static unordered_map<string, int> failedAttempts;

bool PolicyEnforcer::canDecrypt(const string& username) {
    return failedAttempts[username] < 3;
}

bool PolicyEnforcer::canRecoverKey(const string& username) {
    return failedAttempts[username] < 3;
}

void PolicyEnforcer::registerFailedAttempt(const string& username) {
    failedAttempts[username]++;
    AuditLogger::logEvent(
        "Failed attempt " + to_string(failedAttempts[username]) +
        " for user: " + username
    );
}