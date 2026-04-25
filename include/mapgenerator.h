#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include "room.h"
#include "types.h"
#include <vector>
#include <string>

class MapGenerator
{
private:
    int totalRooms;
    int difficulty;
    std::vector<Room *> generatedRooms;
    int shopFrequency;

    // Private helper methods
    bool isBossRoom(int roomNumber);
    bool isLastRoom(int roomNumber);

public:
    MapGenerator();
    ~MapGenerator();

    // Initialization
    void initMapGenerator(int numRooms, int diff);

    // Map/Room generation
    void generateMap();
    Room *generateRoom(int roomId, int roomNumber);
    RoomType determineRoomType(int roomNumber);

    // Getters
    std::vector<Room *> getGeneratedRooms() const;
    int getTotalRooms() const;
    int getDifficulty() const;
    Room *getRoomById(int roomId);

    // Randomization
    bool shouldHaveShop(int roomNumber);
    int getRandomMonsterCount(int difficulty);
    int getRandomTrapCount(int difficulty);

    // Utility
    int countRoomsByType(RoomType type) const;
};

#endif // MAPGENERATOR_H