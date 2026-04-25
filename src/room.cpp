#include <iostream>
#include <sstream>
#include "room.h"
#include "monster.h"
#include "trap.h"
#include "player.h"
#include <cstdlib>

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
    ss << "Type: ";
    switch (roomType)
    {
    case NORMAL:
        ss << "Normal";
        break;
    case BOSS:
        ss << "Boss";
        break;
    case SHOP:
        ss << "Shop";
        break;
    case TREASURE:
        ss << "Treasure";
        break;
    }
    ss << "\nMonsters: " << monsters.size() << "\n";
    ss << "Traps: " << traps.size() << "\n";
    ss << "Shop: " << (hasShop ? "Yes" : "No") << "\n";
    ss << "Cleared: " << (isCleared ? "Yes" : "No") << "\n";
    return ss.str();
}

// Room management
bool Room::clearRoom()
{
    isCleared = true;
    return true;
}

bool Room::isRoomCleared() const
{
    return isCleared;
}

bool Room::hasShopInRoom() const
{
    return hasShop;
}

// Getters
RoomType Room::getRoomType() const
{
    return roomType;
}

int Room::getRoomId() const
{
    return roomId;
}

int Room::getDifficulty() const
{
    return difficulty;
}

std::vector<Monster *> Room::getMonsters() const
{
    return monsters;
}

std::vector<Trap *> Room::getTraps() const
{
    return traps;
}

std::string Room::getDescription() const
{
    return description;
}

// Setters
void Room::setCleared(bool cleared)
{
    isCleared = cleared;
}

void Room::setHasShop(bool shop)
{
    hasShop = shop;
}

void Room::setRoomType(RoomType type)
{
    roomType = type;
}