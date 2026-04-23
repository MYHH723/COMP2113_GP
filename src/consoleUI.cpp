#include <iostream>
#include "consoleUI.h"

void consoleUI::showMainMenu()
{
    std::cout << "=============================\n";
    std::cout << " Sword Magic Dungeon Adventure\n";
    std::cout << "=============================\n";
    std::cout << "Press ENTER to start...\n";
    std::cin.ignore();
    std::cin.get();
}

void consoleUI::showPlayerStatus(const Player &player)
{
    std::cout << "\n=== Player Status ===\n";
    std::cout << "ATK: " << player.get_ATK() << "\n";
    std::cout << "DEF: " << player.get_DEF() << "\n";
    std::cout << "HP: " << player.get_HP() << "\n";
    std::cout << "EXP: " << player.get_EXP() << "\n";
    std::cout << "Money: " << player.get_Money() << "\n";
    std::cout << "Alive: " << (player.get_isAlive() ? "Yes" : "No") << "\n";
    std::cout << "Poisoned: " << (player.get_isPoisoned() ? "Yes" : "No") << "\n";
    std::cout << "Stunned: " << (player.get_isStunned() ? "Yes" : "No") << "\n";
}

void consoleUI::showInventory(const Player &player)
{
    std::cout << "\n=== Inventory ===\n";

    const auto &items = player.get_inventory_items();

    if (items.empty())
    {
        std::cout << "(empty)\n";
        return;
    }

    for (const auto &item : items)
    {
        std::cout << "- " << item << "\n";
    }
}