#ifndef GAMELOGGER_H
#define GAMELOGGER_H

#include "item.h"
#include <string>
#include <fstream>

// GameLogger class - records all transactions and item usage
class GameLogger {
private:
    std::string logPath;
    std::ofstream logFile;

public:
    GameLogger();
    ~GameLogger();

    // Initialization
    void initLogFile();

    // Logging
    void logTransaction(const std::string& action, ItemType type, int grade, int gold);

    // Cleanup
    void closeLogFile();
};

// Global constants
const std::string LOG_FILE = "./shop_transaction.log";

#endif // GAMELOGGER_H
