#include "savegame.h"


// Implementations for Game save/load (new)
// you can write your code and ignore these, but dont delete them
void Game::save() {
    if (player) saveFull(*this, *player);
}
void Game::load() {
    if (player) loadFull(*this, *player);
}

