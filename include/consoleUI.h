#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include <iostream>
#include <vector>
#include <string>
#include "player.h"

class ConsoleUI
{
public:
    static void showPlayerStatus(const Player &player);
    static void showInventory(const Player &player);
    static void showMainMenu();
};

#endif