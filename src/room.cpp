#include <iostream>
#include <sstream>
#include "room.h"
#include "monster.h"
#include "trap.h"
#include "player.h"

// Constructor
Room::Room()
    : roomId(0), difficulty(0), roomType(NORMAL),
      hasShop(false), isCleared(false) {}

// Destructor
Room::~Room()
{
    for (auto m : monsters)
        delete m;
    for (auto t : traps)
        delete t;
}

// initialization of room
void Room::initRoom(int id, int diff, RoomType type)
{
    roomId = id;
    difficulty = diff;
    roomType = type;
    isCleared = false;
    hasShop = (type == SHOP);
}

// room generation logic based on difficulty
void Room::generateRoomContent(int diff)
{
    // Clear any existing content first
    for (auto m : monsters)
        delete m;
    for (auto t : traps)
        delete t;
    monsters.clear();
    traps.clear();

    // Use your difficulty-based constants
    int monsterCount = 0;
    int trapCount = 0;

    switch (diff)
    {
    case 0:                            // Easy
        monsterCount = 1 + rand() % 2; // 1-2
        trapCount = rand() % 2;        // 0-1
        break;
    case 1:                            // Normal
        monsterCount = 1 + rand() % 3; // 1-3
        trapCount = rand() % 3;        // 0-2
        break;
    case 2:                            // Hard
        monsterCount = 2 + rand() % 3; // 2-4
        trapCount = 1 + rand() % 3;    // 1-3
        break;
    }

    // Create monsters (you'll need to integrate with Monster class)
    for (int i = 0; i < monsterCount; i++)
    {
        // monsters.push_back(new Monster(/* params */));
    }

    // Create traps
    for (int i = 0; i < trapCount; i++)
    {
        // traps.push_back(new Trap(/* params */));
    }
}

// Display room information
std::string Room::showRoomInfo() const
{
    std::stringstream ss;
    ss << "Room ID: " << roomId << "\n";
    ss << "Difficulty: " << difficulty << "\n";
    ss << "Type: " << roomType << "\n";
    ss << "Monsters: " << monsters.size() << "\n";
    ss << "Traps: " << traps.size() << "\n";
    return ss.str();
}