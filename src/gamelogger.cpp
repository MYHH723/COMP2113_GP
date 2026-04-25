#include "gamelogger.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

// Constructor
GameLogger::GameLogger() : logPath(LOG_FILE) {}

// Destructor
GameLogger::~GameLogger() {
    closeLogFile();
}

// Initialize and open log file in append mode
void GameLogger::initLogFile() {
    logFile.open(logPath, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file: " << logPath << std::endl;
    }
}

// Log transaction details to file
void GameLogger::logTransaction(const std::string& action, ItemType type, int rarity, int gold) {
    if (!logFile.is_open()) {
        std::cerr << "Log file is not open." << std::endl;
        return;
    }

    // Get current timestamp
    std::time_t now = std::time(nullptr);
    std::string timeStr = std::ctime(&now);

    // Convert ItemType to string
    std::string typeStr;
    switch (type) {
        case CONSUMABLE: typeStr = "Consumable"; break;
        case WEAPON:     typeStr = "Weapon";     break;
        case ARMOR:      typeStr = "Armor";      break;
        default:         typeStr = "Unknown";    break;
    }

    // Write log to file
    logFile << "[" << timeStr.substr(0, timeStr.size() - 1) << "] "
            << "Action: " << action << " | "
            << "ItemType: " << typeStr << " | "
            << "Rarity: " << rarity << " | "
            << "Gold: " << gold << std::endl;
}

// Close the log file
void GameLogger::closeLogFile() {
    if (logFile.is_open()) {
        logFile.close();
    }
}
