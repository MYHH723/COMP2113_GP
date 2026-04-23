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
// void generateRoomContent()

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