#include "AuditLogger.h"
#include <fstream>
#include <ctime>

using namespace std;

void AuditLogger::logEvent(const string& event) {
    ofstream logFile("logs/security.log", ios::app);
    if (!logFile.is_open()) return;

    time_t now = time(nullptr);
    logFile << ctime(&now) << " : " << event << endl;
    logFile.close();
}