/**
 * @file utils.cpp
 * @brief Implementation of utility functions.
 * @author Member 1
 */

#include "utils.h"
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>

void clearScreen() {
    std::cout << "\033[2J\033[1;1H";   // ANSI escape codes for Linux terminal
}

void pause() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

int getRandom(int min, int max) {
    return min + std::rand() % (max - min + 1);
}

void printWithDelay(const std::string& text, int ms) {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
    std::cout << std::endl;
}