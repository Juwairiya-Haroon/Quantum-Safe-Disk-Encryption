#pragma once
#include <string>

using namespace std;

class AuditLogger {
public:
    static void logEvent(const string& event);
};
