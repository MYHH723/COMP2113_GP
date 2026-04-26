// savegame.h
#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Game;  

bool saveGame();
bool loadGame();

#endif 