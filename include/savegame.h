// savegame.h
#ifndef SAVEGAME_H
#define SAVEGAME_H

class Player;  // Forward declarations
class Game;

// Player save/load (existing)
void saveGame(const Player& player);
void loadGame(Player& player);

// Game save/load (new)
void saveGame(const Game& game);
void loadGame(Game& game);

// Combined save/load for full game state
void saveFullGame(const Game& game, const Player& player);
void loadFullGame(Game& game, Player& player);

#endif // SAVEGAME_H