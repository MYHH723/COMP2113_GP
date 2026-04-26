// savegame.cpp
#include "savegame.h"
#include "game.h"
#include "player.h"
#include "mapgenerator.h"
#include <fstream>
#include <iostream>


void Game::saveGame() {
    json saveData;
    saveData["difficulty"] = difficulty;
    saveData["totalRooms"] = totalRooms;
    saveData["currentRoom"] = currentRoomIndex;
    saveData["playerWin"] = playerWin;
    saveData["playerName"] = playerName;
    saveData["player"] = player->toJson();   // Player class must implement toJson()

    std::ofstream file("data/save.json");
    if (file.is_open()) {
        file << saveData.dump(4);   // pretty print with 4 spaces
        file.close();
        std::cout << "Game saved.\n";
    } else {
        std::cerr << "Error: Could not save game.\n";
    }
}

void Game::loadGame() {
    std::ifstream file("data/save.json");
    if (!file.is_open()) {
        std::cout << "No save file found.\n";
        isRunning = false;
        return;
    }

    json saveData;
    file >> saveData;
    file.close();

    // Restore game state
    difficulty = saveData["difficulty"];
    totalRooms = saveData["totalRooms"];
    currentRoomIndex = saveData["currentRoom"];
    playerWin = saveData["playerWin"];
    playerName = saveData["playerName"];

    // Recreate player and load its data
    delete player;
    player = new Player(playerName);
    player->fromJson(saveData["player"]);

    // Recreate map generator and rooms
    delete mapGen;
    mapGen = new MapGenerator(totalRooms, difficulty);
    rooms = mapGen->generateAll();   // Note: room contents may differ from saved state

    isRunning = true;
    std::cout << "Game loaded. Welcome back, " << playerName << "!\n";
    pause();
}