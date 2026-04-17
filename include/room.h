#ifndef ROOM_H
#define ROOM_H

#include "item.h"
#include <string>
#include <vector>

// Forward declarations
class Monster;
class Trap;
class Player;

// Room class - represents a single game room
class Room {
private:
    int roomId;
    int difficulty;
    std::vector<Monster*> monsters;
    std::vector<Trap*> traps;
    bool hasShop;
    bool isCleared;
    std::string description;

public:
    Room();
    ~Room();

    // Initialization
    void initRoom(int id, int diff);
    void generateRoomContent(int diff);

    // Room management
    bool clearRoom();
    bool isRoomCleared() const;
    bool hasShopInRoom() const;

    // Getters
    int getRoomId() const;
    int getDifficulty() const;
    std::vector<Monster*> getMonsters() const;
    std::vector<Trap*> getTraps() const;
    std::string getDescription() const;

    // Setters
    void setCleared(bool cleared);
    void setHasShop(bool shop);

    // Display
    std::string showRoomInfo();
};

#endif // ROOM_H
