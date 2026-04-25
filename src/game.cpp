#include "savegame.h"  

bool Game::save() {
    if (!player) {
        return false;
    }
    return saveGame(*this, *player); 
}

bool Game::load() {
    if (!player) {
        return false;
    }
    return loadGame(*this, *player); 
}