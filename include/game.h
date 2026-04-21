#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>

// Forward declarations
class Player;
class Room;

// Game class - manages overall game flow and state
class Game {
private:
    int difficulty;         // 0=Easy, 1=Normal, 2=Hard
    int totalRooms;         // Total rooms to clear
    int currentRoom;        // Current room index
    bool isRunning;         // Game is active
    bool playerWin;         // Player completed all rooms
    std::string playerName; // Player name
    Player* player;         // Pointer to player object
    std::vector<Room*> rooms; // All rooms in current playthrough
    std::vector<int> clearedRoomIds;  // Track which rooms player has cleared
    unsigned mapSeed;                 // seed for map generation
public:
    Game();
    ~Game();

    // Menu & Initialization
    void showMainMenu();
    void selectDifficulty();
    void initGame();

    // Game core loop
    void gameLoop();
    void enterNextRoom();

    // Game end logic
    void checkGameOver();
    void showGameResult();

    // Getters
    int get_difficulty() const;
    int get_totalRooms() const;
    int get_currentRoom() const;
    bool get_isRunning() const;
    bool get_playerWin() const;
    std::string get_playerName() const;
    std::vector<int> getClearedRooms() const;  // Returns IDs of cleared rooms
    unsigned getRandomSeed() const;

    // Setters
    void set_difficulty(int diff);
    void set_currentRoom(int room);
    void set_isRunning(bool running);
    void set_totalRooms(int rooms);
    void set_playerName(const std::string& name);
    void set_playerWin(bool win);
    void setClearedRooms(const std::vector<int>& clearedIds);
    void setRandomSeed(unsigned seed);//for reproducible generation
    
};

// Utility functions (can be in utils.h separately if needed)
namespace GameUtils {
    void clearScreen();
    void pause();
    int getRandom(int min, int max);
    void printWithDelay(const std::string& text, int ms);
}

// Global constants
const int MAX_ROOMS_EASY = 10;
const int MAX_ROOMS_NORMAL = 15;
const int MAX_ROOMS_HARD = 20;
const float MOB_HP_MULTIPLIER[3] = { 0.8f, 1.0f, 1.3f };
const int TRAP_DMG_MIN[3] = { 10, 15, 25 };
const int TRAP_DMG_MAX[3] = { 20, 30, 40 };

#endif // GAME_H
