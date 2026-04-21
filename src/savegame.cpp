// savegame.cpp
#include "savegame.h"
#include "player.h"
#include "game.h"
#include <iostream>
#include <fstream>
#include <string>

// Helper: write a key:value line
static void writeLine(std::ofstream& file, const std::string& key, const std::string& value) {
    file << key << ":" << value << "\n";
}

void saveGame(const Player& player) {
    std::ofstream file(SAVE_FILE);  // Uses global constant from player.h
    if (!file.is_open()) {
        std::cerr << "Failed to open save file." << std::endl;
        return;
    }

    // === Player Stats ===
    writeLine(file, "ATK", std::to_string(player.get_ATK()));
    writeLine(file, "DEF", std::to_string(player.get_DEF()));
    writeLine(file, "HP", std::to_string(player.get_HP()));
    writeLine(file, "EXP", std::to_string(player.get_EXP()));
    writeLine(file, "Money", std::to_string(player.get_Money()));
    
    // === Status Flags ===
    writeLine(file, "isAlive", player.get_isAlive() ? "1" : "0");
    writeLine(file, "isPoisoned", player.get_isPoisoned() ? "1" : "0");
    writeLine(file, "isStunned", player.get_isStunned() ? "1" : "0");
    
    // === Inventory (preserves your exact string format) ===
    file << "[INVENTORY]\n";
    for (const auto& itemStr : player.inventory->get_items()) {  // Requires friend access (see Step 5)
        writeLine(file, "ITEM", itemStr);
    }
    file << "[/INVENTORY]\n";
    
    file.close();
    std::cout << "Game saved." << std::endl;
}

void loadGame(Player& player) {
    std::ifstream file(SAVE_FILE);
    if (!file.is_open()) {
        std::cerr << "Failed to open save file." << std::endl;
        return;
    }

    std::string line;
    bool inInventory = false;
    player.inventory->clear_items();  // Reset before loading
    
    while (std::getline(file, line)) {
        if (line == "[INVENTORY]") { inInventory = true; continue; }
        if (line == "[/INVENTORY]") { inInventory = false; continue; }
        
        if (inInventory && line.rfind("ITEM:", 0) == 0) {
            std::string itemData = line.substr(5);  // Remove "ITEM:"
            player.inventory->add_item(itemData);   // Preserves your format
            continue;
        }
        
        size_t pos = line.find(':');
        if (pos == std::string::npos) continue;
        
        std::string key = line.substr(0, pos);
        std::string valStr = line.substr(pos + 1);
        
        if (key == "isAlive") player.set_isAlive(valStr == "1");
        else if (key == "isPoisoned") player.set_isPoisoned(valStr == "1");
        else if (key == "isStunned") player.set_isStunned(valStr == "1");
        else player.change_state(key, std::stof(valStr));  // ATK/DEF/HP/EXP/Money
    }
    
    file.close();
    std::cout << "Game loaded." << std::endl;
}
// Helper to write key:value
static void writeKV(std::ofstream& f, const std::string& k, const std::string& v) {
    f << k << ":" << v << "\n";
}

void saveGame(const Game& game) {
    std::ofstream file(SAVE_FILE, std::ios::app);
    if (!file.is_open()) { std::cerr << "Save failed: cannot open file\n"; return; }

    file << "[GAME_STATE]\n";
    writeKV(file, "difficulty", std::to_string(game.get_difficulty()));
    writeKV(file, "totalRooms", std::to_string(game.get_totalRooms()));
    writeKV(file, "currentRoom", std::to_string(game.get_currentRoom()));
    writeKV(file, "isRunning", game.get_isRunning() ? "1" : "0");
    writeKV(file, "playerWin", game.get_playerWin() ? "1" : "0");
    writeKV(file, "playerName", game.get_playerName());
    writeKV(file, "mapSeed", std::to_string(game.getRandomSeed()));  // optional
    
    // Save cleared room IDs (the ONLY room state we persist)
    auto cleared = game.getClearedRooms();
    for (int id : cleared) {
        writeKV(file, "clearedRoom", std::to_string(id));
    }
    file << "[/GAME_STATE]\n";
    file.close();
}

void loadGame(Game& game) {
    std::ifstream file(SAVE_FILE);
    if (!file.is_open()) { std::cerr << "Load failed: cannot open file\n"; return; }

    std::string line;
    bool inSection = false;
    std::vector<int> clearedIds;
    
    while (std::getline(file, line)) {
        if (line == "[GAME_STATE]") { inSection = true; continue; }
        if (line == "[/GAME_STATE]" || line.rfind("[", 0) == 0) { inSection = false; continue; }
        if (!inSection) continue;
        
        auto pos = line.find(':');
        if (pos == std::string::npos) continue;
        std::string key = line.substr(0, pos);
        std::string val = line.substr(pos + 1);
        
        if (key == "difficulty") game.set_difficulty(std::stoi(val));
        else if (key == "totalRooms") game.set_totalRooms(std::stoi(val));
        else if (key == "currentRoom") game.set_currentRoom(std::stoi(val));
        else if (key == "isRunning") game.set_isRunning(val == "1");
        else if (key == "playerWin") game.set_playerWin(val == "1");
        else if (key == "playerName") game.set_playerName(val);
        else if (key == "mapSeed") game.setRandomSeed(std::stoul(val));
        else if (key == "clearedRoom") clearedIds.push_back(std::stoi(val));
    }
    file.close();
    
    // Apply cleared rooms AFTER map is regenerated (caller must call generateMap first)
    game.setClearedRooms(clearedIds);
}