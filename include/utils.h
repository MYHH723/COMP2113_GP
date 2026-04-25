/**
 * @file utils.h
 * @brief Utility functions for UI and randomness.
 * @author Member 1
 */

#ifndef UTILS_H
#define UTILS_H

#include <string>

void clearScreen();                       // clear terminal (Linux)
void pause();                             // wait for user input
int getRandom(int min, int max);          // inclusive random int
void printWithDelay(const std::string& text, int ms); // slow print

#endif