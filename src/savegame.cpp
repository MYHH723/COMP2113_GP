// savegame.cpp
#include "savegame.h"
#include "player.h"
#include "game.h"
#include <iostream>
#include <fstream>
#include <string>

static void writeKV(std::ofstream& f, const std::string& k, const std::string& v) {
    f << k << ":" << v << "\n";
}

// ========== UNIFIED SAVE ==========
bool saveGame(const Game& game, const Player& player) {
    std::ofstream file(SAVE_FILE);
    if (!file.is_open()) {
        return false;
    }

    // === Player Stats ===
    writeKV(file, "ATK", std::to_string(player.get_ATK()));
    writeKV(file, "DEF", std::to_string(player.get_DEF()));
    writeKV(file, "HP", std::to_string(player.get_HP()));
    writeKV(file, "EXP", std::to_string(player.get_EXP()));
    writeKV(file, "Money", std::to_string(player.get_Money()));
    
    // === Player Status Flags ===
    writeKV(file, "isAlive", player.get_isAlive() ? "1" : "0");
    writeKV(file, "isPoisoned", player.get_isPoisoned() ? "1" : "0");
    writeKV(file, "isStunned", player.get_isStunned() ? "1" : "0");
    
    // === Player Inventory ===
    file << "[INVENTORY]\n";
    for (const auto& itemStr : player.getInventory()->get_items()) {
        writeKV(file, "ITEM", itemStr);
    }
    file << "[/INVENTORY]\n";

    // === Game State ===
    file << "[GAME]\n";
    writeKV(file, "difficulty", std::to_string(game.get_difficulty()));
    writeKV(file, "totalRooms", std::to_string(game.get_totalRooms()));
    writeKV(file, "currentRoom", std::to_string(game.get_currentRoom()));
    writeKV(file, "isRunning", game.get_isRunning() ? "1" : "0");
    writeKV(file, "playerWin", game.get_playerWin() ? "1" : "0");
    writeKV(file, "playerName", game.get_playerName());
    writeKV(file, "mapSeed", std::to_string(game.getRandomSeed()));
    
    // === Cleared Rooms ===
    for (int id : game.getClearedRooms()) {
        writeKV(file, "clearedRoom", std::to_string(id));
    }
    file << "[/GAME]\n";
    
    if (file.fail()) {
        std::cerr << "Failed to write save file." << std::endl;
        return false;
    }
    
    file.close();
    std::cout << "Game saved." << std::endl;
    return true;
}

// ========== UNIFIED LOAD ==========
bool loadGame(Game& game, Player& player) {
    std::ifstream file(SAVE_FILE);
    if (!file.is_open()) {
        std::cerr << "Failed to open save file." << std::endl;
        return false;
    }

    std::string line;
    std::string currentSection;
    std::vector<int> clearedIds;
    
    player.getInventory()->clear_items();

    while (std::getline(file, line)) {
        if (line == "[INVENTORY]") { currentSection = "INVENTORY"; continue; }
        if (line == "[/INVENTORY]") { currentSection = ""; continue; }
        if (line == "[GAME]") { currentSection = "GAME"; continue; }
        if (line == "[/GAME]") { currentSection = ""; continue; }
        
        size_t pos = line.find(':');
        if (pos == std::string::npos) continue;
        std::string key = line.substr(0, pos);
        std::string val = line.substr(pos + 1);

        if (currentSection == "INVENTORY" && key == "ITEM") {
            player.getInventory()->add_item(val);
            continue;
        }
        
        if (currentSection == "GAME") {
            if (key == "difficulty") game.set_difficulty(std::stoi(val));
            else if (key == "totalRooms") game.set_totalRooms(std::stoi(val));
            else if (key == "currentRoom") game.set_currentRoom(std::stoi(val));
            else if (key == "isRunning") game.set_isRunning(val == "1");
            else if (key == "playerWin") game.set_playerWin(val == "1");
            else if (key == "playerName") game.set_playerName(val);
            else if (key == "mapSeed") game.setRandomSeed(std::stoul(val));
            else if (key == "clearedRoom") clearedIds.push_back(std::stoi(val));
            continue;
        }
        
        if (currentSection.empty()) {
            if (key == "isAlive") player.set_isAlive(val == "1");
            else if (key == "isPoisoned") player.set_isPoisoned(val == "1");
            else if (key == "isStunned") player.set_isStunned(val == "1");
            else player.change_state(key, std::stof(val));
        }
    }
    
    file.close();
    game.setClearedRooms(clearedIds);
    
    std::cout << "Game loaded." << std::endl;
    return true;
}