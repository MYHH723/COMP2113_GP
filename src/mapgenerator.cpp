#include <cstdlib>
#include <algorithm>
#include "mapgenerator.h"

// Constructor
MapGenerator::MapGenerator()
    : totalRooms(0), difficulty(0), shopFrequency(0) {}

// Destructor
MapGenerator::~MapGenerator()
{
    for (auto room : generatedRooms)
        delete room;
}

void MapGenerator::initMapGenerator(int numRooms, int diff)
{
    totalRooms = numRooms;
    difficulty = diff;

    // Set shop frequency based on difficulty
    switch (difficulty)
    {
    case 0: // Easy
        shopFrequency = 3;
        break;
    case 1: // Normal
        shopFrequency = 4;
        break;
    case 2: // Hard
        shopFrequency = 5;
        break;
    default:
        shopFrequency = 4;
    }
}

void MapGenerator::generateMap()
{
    // Clear previous map
    for (auto room : generatedRooms)
        delete room;
    generatedRooms.clear();

    // Generate each room
    for (int i = 0; i < totalRooms; i++)
    {
        Room *room = generateRoom(i, i);
        generatedRooms.push_back(room);
    }
}

Room *MapGenerator::generateRoom(int roomId, int roomNumber)
{
    Room *room = new Room();
    RoomType type = determineRoomType(roomNumber);

    room->initRoom(roomId, difficulty, type);
    room->generateRoomContent(difficulty);

    // Handle shop assignment
    if (shouldHaveShop(roomNumber))
    {
        room->setHasShop(true);
    }

    return room;
}

RoomType MapGenerator::determineRoomType(int roomNumber)
{
    // Last room is always BOSS or TREASURE
    if (isLastRoom(roomNumber))
    {
        // 50% chance for boss, 50% for treasure in final room
        return (rand() % 2 == 0) ? BOSS : TREASURE;
    }

    // Check if this should be a boss room based on difficulty
    if (isBossRoom(roomNumber))
    {
        return BOSS;
    }

    // Check for shop rooms
    if (shouldHaveShop(roomNumber))
    {
        // 70% shop, 30% treasure
        return (rand() % 100 < 70) ? SHOP : TREASURE;
    }

    // 10% chance for treasure room in random positions
    if (rand() % 100 < 10)
    {
        return TREASURE;
    }

    // Default: normal combat room
    return NORMAL;
}

bool MapGenerator::isBossRoom(int roomNumber)
{
    switch (difficulty)
    {
    case 0: // Easy: 1 boss at room 7 (0-indexed)
        return (roomNumber == 7);

    case 1: // Normal: 2 bosses at rooms 6 and 13 (0-indexed)
        return (roomNumber == 6 || roomNumber == 13);

    case 2: // Hard: 3 bosses at rooms 5, 11, and 18 (0-indexed)
        return (roomNumber == 5 || roomNumber == 11 || roomNumber == 18);

    default:
        return false;
    }
}

bool MapGenerator::isLastRoom(int roomNumber)
{
    return (roomNumber == totalRooms - 1);
}

bool MapGenerator::shouldHaveShop(int roomNumber)
{
    // Skip first room (starting room) and boss rooms
    if (roomNumber == 0 || isBossRoom(roomNumber))
        return false;

    // Last room shouldn't be a shop
    if (isLastRoom(roomNumber))
        return false;

    // Shop every N rooms (with some randomness)
    int baseFrequency = shopFrequency;
    int variation = (rand() % 2); // +/- 1 variation

    if (roomNumber % (baseFrequency + variation) == 0)
        return true;

    return false;
}

int MapGenerator::getRandomMonsterCount(int difficulty)
{
    switch (difficulty)
    {
    case 0: // Easy: 1-2 monsters
        return 1 + rand() % 2;
    case 1: // Normal: 1-3 monsters
        return 1 + rand() % 3;
    case 2: // Hard: 2-4 monsters
        return 2 + rand() % 3;
    default:
        return 1 + rand() % 2;
    }
}

int MapGenerator::getRandomTrapCount(int difficulty)
{
    switch (difficulty)
    {
    case 0: // Easy: 0-1 traps
        return rand() % 2;
    case 1: // Normal: 0-2 traps
        return rand() % 3;
    case 2: // Hard: 1-3 traps
        return 1 + rand() % 3;
    default:
        return rand() % 2;
    }
}

// Getters
std::vector<Room *> MapGenerator::getGeneratedRooms() const
{
    return generatedRooms;
}

int MapGenerator::getTotalRooms() const
{
    return totalRooms;
}

Room *MapGenerator::getRoomById(int roomId)
{
    for (auto room : generatedRooms)
    {
        if (room->getRoomId() == roomId)
            return room;
    }
    return nullptr;
}