/*#include "game.h"
#include "player.h"
#include <iostream>
#include <string>
#include "consoleUI.h"

// Demo main function to test Player and ConsoleUI
int main()
{
    try
    {
        Player player;
        Game game;

        game.set_playerName("Hero");
        game.set_difficulty(1);

        ConsoleUI::showMainMenu();

        bool running = true;
        while (running)
        {
            std::cout << "\n===== GAME MENU =====\n";
            std::cout << "1. Show Status\n";
            std::cout << "2. Show Inventory\n";
            std::cout << "3. Exit\n";
            std::cout << "Choose: ";

            int choice;
            std::cin >> choice;

            switch (choice)
            {
            case 1:
                ConsoleUI::showPlayerStatus(player);
                break;
            case 2:
                ConsoleUI::showInventory(player);
                break;
            case 3:
                running = false;
                break;
            default:
                std::cout << "Invalid choice\n";
            }
        }

        std::cout << "Game closed.\n";
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}*/

#include "game.h"
using namespace std;

// Main function: Program entry point
int main() {
    Game game;
    game.showMainMenu();
    game.gameLoop();
    return 0;
}
