#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include <vector>
#include <string>

// Forward declaration
class Room;

// MapGenerator class - generates rooms and handles room layout
class MapGenerator {
private:
    int totalRooms;
    int difficulty;
    std::vector<Room*> generatedRooms;
    int shopFrequency;  // How often shops appear

public:
    MapGenerator();
    ~MapGenerator();

    // Initialization
    void initMapGenerator(int numRooms, int diff);

    // Map/Room generation
    void generateMap();
    Room* generateRoom(int roomId, int roomNumber);
    void determineRoomType(Room* room, int roomNumber);

    // Getters
    std::vector<Room*> getGeneratedRooms() const;
    int getTotalRooms() const;
    int getDifficulty() const;
    Room* getRoomById(int roomId);

    // Randomization
    bool shouldHaveShop(int roomNumber);
    int getRandomMonsterCount(int difficulty);
    int getRandomTrapCount(int difficulty);
};

#endif // MAPGENERATOR_H
