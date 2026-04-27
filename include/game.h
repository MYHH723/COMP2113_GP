// game.h
// Declaration of the Game class – controls the whole game flow.

#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>

// Forward declarations of modules implemented by other team members
class Player;
class MapGenerator;
class Room;
class BattleSystem;
class Shop;

class Game {
private:
    // Game state variables
    int difficulty;          // 0 = Easy, 1 = Normal, 2 = Hard
    int totalRooms;          // Number of rooms to clear (depends on difficulty)
    int currentRoomIndex;    // Index of the room the player is in (0-based)
    bool isRunning;          // True while the game is active
    bool playerWin;          // True if player cleared all rooms

    Player* player;          // Player object (dynamically allocated)

    // Map and rooms
    MapGenerator* mapGen;    // Generates the dungeon layout
    std::vector<Room*> rooms; // List of all rooms in the current game

    // Private helper methods
    void generateRooms();    // Uses MapGenerator to create all rooms
    void applyDifficultyScaling(); // Sets global modifiers for monsters/traps

public:
    Game();                  // Constructor
    ~Game();                 // Destructor (cleans up dynamic memory)

    // Menu and initialization
    void showMainMenu();     // Displays new game, load game, exit
    void selectDifficulty(); // Lets player choose difficulty
    void initGame();         // Starts a new game with chosen difficulty

    // Main game loop
    void gameLoop();         // Runs the core gameplay
    void enterNextRoom();    // Enters the next room and processes its event

    // Game ending
    void checkGameOver();    // Checks if player died or finished all rooms
    void showGameResult();   // Shows win / lose screen

    // Save / Load (JSON format)
    void saveGame();         // Saves current progress to data/save.json
    void loadGame();         // Loads saved game from data/save.json
};

#endif
