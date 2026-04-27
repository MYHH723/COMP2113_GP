#include "savegame.h"  
// game.cpp
// Implementation of the Game class.

#include "game.h"
#include "player.h"
#include "mapgenerator.h"
#include "room.h"
#include "battlesystem.h"
#include "shop.h"
#include "utils.h"
#include "merchant.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <nlohmann/json.hpp>  // JSON library – make sure it's included in your project

using json = nlohmann::json;

// ========== Difficulty constants (matching README) ==========
const int MAX_ROOMS_EASY   = 10;
const int MAX_ROOMS_NORMAL = 15;
const int MAX_ROOMS_HARD   = 20;

const float MONSTER_HP_MULTIPLIER[3] = {0.8f, 1.0f, 1.3f};
const int TRAP_DAMAGE_MIN[3] = {5, 15, 30};   // Easy, Normal, Hard
const int TRAP_DAMAGE_MAX[3] = {15, 30, 50};

// ========== Constructor & Destructor ==========
Game::Game()
    : difficulty(1), totalRooms(MAX_ROOMS_NORMAL), currentRoomIndex(0),
      isRunning(false), playerWin(false), player(nullptr), mapGen(nullptr) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

Game::~Game() {
    delete player;
    delete mapGen;
    for (Room* r : rooms) delete r;
    rooms.clear();
}

// ========== Menu and Initialization ==========
void Game::showMainMenu() {
    int choice = 0;
    while (true) {
        clearScreen();
        std::cout << "\n================================\n";
        std::cout << "     ROGUELIKE DUNGEON GAME\n";
        std::cout << "================================\n";
        std::cout << "1. New Game\n";
        std::cout << "2. Load Game\n";
        std::cout << "3. Exit\n";
        std::cout << "Choose: ";
        std::cin >> choice;

        if (choice == 1) {
            selectDifficulty();
            initGame();
            gameLoop();
        } else if (choice == 2) {
            loadGame();
            if (isRunning) {
                gameLoop();
            } else {
                std::cout << "Load failed or no save file.\n";
                pause();
            }
        } else if (choice == 3) {
            std::cout << "Goodbye!\n";
            break;
        } else {
            std::cout << "Invalid choice.\n";
            pause();
        }
    }
}

void Game::selectDifficulty() {
    clearScreen();
    std::cout << "\n=== DIFFICULTY ===\n";
    std::cout << "0. Easy   (10 rooms, weak monsters, low trap damage)\n";
    std::cout << "1. Normal (15 rooms, standard)\n";
    std::cout << "2. Hard   (20 rooms, tough monsters, high trap damage)\n";
    std::cout << "Choice: ";
    std::cin >> difficulty;
    if (difficulty < 0 || difficulty > 2) difficulty = 1;

    switch (difficulty) {
        case 0: totalRooms = MAX_ROOMS_EASY; break;
        case 1: totalRooms = MAX_ROOMS_NORMAL; break;
        case 2: totalRooms = MAX_ROOMS_HARD; break;
    }
    std::cout << "Difficulty set. Total rooms: " << totalRooms << "\n";
    pause();
}

void Game::initGame() {
    // Clean up previous game data (if any)
    delete player;
    delete mapGen;
    for (Room* r : rooms) delete r;
    rooms.clear();

    // Get player name
    std::string name;
    clearScreen();
    std::cout << "Enter your name: ";
    std::cin >> name;
    playerName = name;

    // Create player (stats follow README: HP=1000, ATK=100, DEF=100, starting gold=500)
    player = new Player(playerName);

    // Create map generator
    mapGen = new MapGenerator();

    // Apply difficulty scaling (sets global modifiers for monsters/traps)
    applyDifficultyScaling();

    // Generate all rooms using the map generator
    generateRooms();

    currentRoomIndex = 0;
    isRunning = true;
    playerWin = false;

    std::cout << "\nGame started! Good luck, " << playerName << "!\n";
    pause();
}

// ========== Global difficulty scaling variables ==========
float g_monsterHpMultiplier = 1.0f;
int g_trapDamageMin = 0;
int g_trapDamageMax = 0;

void Game::applyDifficultyScaling() {
    // Monster HP multipliers (Easy: 0.8, Normal: 1.0, Hard: 1.3)
    const float MONSTER_HP_MULT[] = {0.8f, 1.0f, 1.3f};

    // Trap damage ranges (Increased to be meaningful for 1000 HP)
    // Easy:    80 - 150
    // Normal: 150 - 280
    // Hard:   250 - 450
    const int TRAP_DAMAGE_MIN[] = {80, 150, 250};
    const int TRAP_DAMAGE_MAX[] = {150, 280, 450};

    // Set global variables based on current difficulty
    g_monsterHpMultiplier = MONSTER_HP_MULT[difficulty];
    g_trapDamageMin = TRAP_DAMAGE_MIN[difficulty];
    g_trapDamageMax = TRAP_DAMAGE_MAX[difficulty];
}

void Game::generateRooms() {
    // Ask MapGenerator to build the whole dungeon.
    // The method generateAll() returns a vector of Room pointers.
    mapGen->initMapGenerator(totalRooms, difficulty);
    rooms = mapGen->getGeneratedRooms();
}

// ========== Main Game Loop ==========
void Game::gameLoop() {
    while (isRunning && currentRoomIndex < totalRooms) {
        clearScreen();
        std::cout << "\n===== Room " << (currentRoomIndex + 1) << " / " << totalRooms << " =====\n";

        // Show player status (using Player's display method)
        player->show_status();

        // Process the current room
        enterNextRoom();

        // Check if game should continue
        if (!isRunning) break;
        if (currentRoomIndex >= totalRooms) {
            playerWin = true;
            break;
        }
        pause();
    }
    checkGameOver();
    showGameResult();
}

void Game::enterNextRoom() {
    if (currentRoomIndex >= static_cast<int>(rooms.size())) return;

    Room* currentRoom = rooms[currentRoomIndex];
    currentRoomIndex++;

    // Determine room type and act accordingly
    RoomType type = currentRoom->getRoomType();  // expects NORMAL, BOSS, SHOP, TREASURE

    switch (type) {
        case RoomType::NORMAL:
        case RoomType::BOSS: {
            std::vector<Monster*> monsters = currentRoom->getMonsters();
            BattleSystem* battle = new BattleSystem();
            BattleResult result = BattleResult::ONGOING;
            std::string battleLog;
            for (auto monster : monsters)
            {
                battle->initBattle(player, monster);
                battle->startBattle();
                while(battle->get_isBattleActive())
                {
                    result = battle->executeBattleRound(); 
                }
                battle->endBattle();
                if(result == BattleResult::PLAYER_LOSE) {
                    break;  // player died, exit loop
                }
            }
            result =  battle->getLastResult(); // returns WIN, LOSE, or FLEE
            battleLog = battle->showBattleLog();
            if (result == BattleResult::PLAYER_LOSE) {
                isRunning = false;
                playerWin = false;
                return;
            }
            else if (result == BattleResult::PLAYER_FLEE) {
                playerWin = false;
                std::cout << battleLog << std::endl;
                battle->applyRewards(); 
                return;
            }
            else if (result == BattleResult::PLAYER_WIN) {
                playerWin = true;
                std::cout << battleLog << std::endl;
                battle->applyRewards(); 
            }
            break;
        }
        case RoomType::SHOP: {
            Shop* shop = new Shop();
            shop->initShop(new Merchant(difficulty), player);
            break;
        }
        case RoomType::TREASURE: {
            // Treasure room: give random item or gold
            // (Implementation depends on your item system)
            break;
        }
        default:
            break;
    }

    // Auto-save after each room (optional but recommended)
    saveGame();
}

// ========== Game End Logic ==========
void Game::checkGameOver() {
    if (player->get_HP() <= 0) {
        isRunning = false;
        playerWin = false;
    } else if (currentRoomIndex >= totalRooms) {
        playerWin = true;
        isRunning = false;
    }
}

void Game::showGameResult() {
    clearScreen();
    if (playerWin) {
        std::cout << "\n================================\n";
        std::cout << "         YOU WIN!\n";
        std::cout << "================================\n";
        std::cout << "Congratulations, " << playerName << "!\n";
        std::cout << "You cleared all " << totalRooms << " rooms.\n";
    } else {
        std::cout << "\n================================\n";
        std::cout << "         GAME OVER\n";
        std::cout << "================================\n";
        std::cout << "You died in room " << currentRoomIndex << ".\n";
    }
    player->show_status();
    pause();
}

/*
// ========== Save / Load (JSON) ==========
void Game::saveGame() {
    json saveData;
    saveData["difficulty"] = difficulty;
    saveData["totalRooms"] = totalRooms;
    saveData["currentRoom"] = currentRoomIndex;
    saveData["playerWin"] = playerWin;
    saveData["playerName"] = playerName;
    saveData["player"] = player->toJson();   // Player class must implement toJson()

    std::ofstream file("data/save.json");
    if (file.is_open()) {
        file << saveData.dump(4);   // pretty print with 4 spaces
        file.close();
        std::cout << "Game saved.\n";
    } else {
        std::cerr << "Error: Could not save game.\n";
    }
}

void Game::loadGame() {
    std::ifstream file("data/save.json");
    if (!file.is_open()) {
        std::cout << "No save file found.\n";
        isRunning = false;
        return;
    }

    json saveData;
    file >> saveData;
    file.close();

    // Restore game state
    difficulty = saveData["difficulty"];
    totalRooms = saveData["totalRooms"];
    currentRoomIndex = saveData["currentRoom"];
    playerWin = saveData["playerWin"];
    playerName = saveData["playerName"];

    // Recreate player and load its data
    delete player;
    player = new Player(playerName);
    player->fromJson(saveData["player"]);

    // Recreate map generator and rooms
    delete mapGen;
    mapGen = new MapGenerator(totalRooms, difficulty);
    rooms = mapGen->generateAll();   // Note: room contents may differ from saved state

    isRunning = true;
    std::cout << "Game loaded. Welcome back, " << playerName << "!\n";
    pause();
}
*/

