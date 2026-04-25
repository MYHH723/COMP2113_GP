// savegame.h
#ifndef SAVEGAME_H
#define SAVEGAME_H

class Player;
class Game;

bool saveGame(const Game& game, const Player& player);
bool loadGame(Game& game, Player& player);

#endif 