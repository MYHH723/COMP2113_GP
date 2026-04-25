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

    // Variables for monster and trap counts
    int monsterCount = 0;
    int trapCount = 0;

    // Determine counts based on room type and difficulty
    switch (roomType)
    {
    case NORMAL:
        // Normal rooms: 1-3 monsters, 0-2 traps (based on difficulty)
        switch (diff)
        {
        case 0: // Easy: 1-2 monsters, 0-1 traps
            monsterCount = 1 + rand() % 2;
            trapCount = rand() % 2;
            break;
        case 1: // Normal: 1-3 monsters, 0-2 traps
            monsterCount = 1 + rand() % 3;
            trapCount = rand() % 3;
            break;
        case 2: // Hard: 2-4 monsters, 1-3 traps
            monsterCount = 2 + rand() % 3;
            trapCount = 1 + rand() % 3;
            break;
        }
        break;

    case BOSS:
        // Boss rooms: 1 powerful monster, 1-2 traps
        monsterCount = 1;
        trapCount = 1 + rand() % 2;
        break;

    case SHOP:
        // Shop rooms: no monsters/traps
        monsterCount = 0;
        trapCount = 0;
        break;

    case TREASURE:
        // Treasure rooms: 0-1 weak monster, no traps
        monsterCount = rand() % 2; // 0 or 1
        trapCount = 0;
        break;
    }

    // Create monsters
    for (int i = 0; i < monsterCount; i++)
    {
        Monster *monster = new Monster();
        monster->initMonster(diff, diff + 1); // level based on difficulty
        monsters.push_back(monster);
    }

    // Create traps
    for (int i = 0; i < trapCount; i++)
    {
        Trap *trap = new Trap();

        // Randomly select trap type
        TrapType type;
        int trapRoll = rand() % 4;
        switch (trapRoll)
        {
        case 0:
            type = TrapType::SPIKE_PIT;
            break;
        case 1:
            type = TrapType::POISON_GAS;
            break;
        case 2:
            type = TrapType::FALLING_BLOCK;
            break;
        case 3:
            type = TrapType::FIRE_BLAST;
            break;
        }

        trap->initTrap(type, diff);
        traps.push_back(trap);
    }

    // Update room description based on content
    std::ostringstream desc;
    switch (roomType)
    {
    case NORMAL:
        desc << "A dimly lit chamber with " << monsterCount << " enemies lurking.";
        break;
    case BOSS:
        desc << "A grand hall. A powerful foe awaits!";
        break;
    case SHOP:
        desc << "A quiet room with a merchant's stall.";
        break;
    case TREASURE:
        desc << "A glimmering treasure room. Riches await!";
        break;
    }
    description = desc.str();
}

// Display room information
std::string Room::showRoomInfo() const
{
    std::ostringstream ss;
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