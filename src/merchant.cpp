#include "merchant.h"
#include <iostream>
#include <string>

// Constructor: Initialize merchant with game difficulty and set default availability
Merchant::Merchant(int gameDiff)
    : currentDiff(gameDiff), isAvailable(true) {
    initMerchant();
}

// Destructor
Merchant::~Merchant() = default;

// Initialize all merchant goods with 3 rarity levels per item type
void Merchant::initMerchant() {
    // Initialize Consumable Items (Potion)
    goods[CONSUMABLE][0] = Item("Low Potion", CONSUMABLE, LOW, LOW_PRICE, LOW_HEAL, 0, 0);
    goods[CONSUMABLE][1] = Item("Medium Potion", CONSUMABLE, MEDIUM, MEDIUM_PRICE, MEDIUM_HEAL, 0, 0);
    goods[CONSUMABLE][2] = Item("High Potion", CONSUMABLE, HIGH, HIGH_PRICE, HIGH_HEAL, 0, 0);

    // Initialize Weapon Items (Sword)
    goods[WEAPON][0] = Item("Iron Sword", WEAPON, LOW, LOW_PRICE, 0, LOW_ATTACK, 0);
    goods[WEAPON][1] = Item("Steel Sword", WEAPON, MEDIUM, MEDIUM_PRICE, 0, MEDIUM_ATTACK, 0);
    goods[WEAPON][2] = Item("Golden Sword", WEAPON, HIGH, HIGH_PRICE, 0, HIGH_ATTACK, 0);

    // Initialize Armor Items
    goods[ARMOR][0] = Item("Cloth Armor", ARMOR, LOW, LOW_PRICE, 0, 0, LOW_DEFENSE);
    goods[ARMOR][1] = Item("Iron Armor", ARMOR, MEDIUM, MEDIUM_PRICE, 0, 0, MEDIUM_DEFENSE);
    goods[ARMOR][2] = Item("Dragon Armor", ARMOR, HIGH, HIGH_PRICE, 0, 0, HIGH_DEFENSE);
}

// Check if the merchant has the specified item type and rarity index
bool Merchant::hasItem(ItemType type, int rarityIndex) const {
    if (rarityIndex < 0 || rarityIndex >= 3) {
        return false;
    }
    return true;
}

// Get the item from goods by type and rarity index
Item Merchant::getItem(ItemType type, int rarityIndex) const {
    if (hasItem(type, rarityIndex)) {
        return goods.at(type).at(rarityIndex);
    }
    // Return default empty item if not found
    return Item();
}

// Show all goods list with detailed information
void Merchant::showGoodsList() const {
    std::cout << "\n===== Merchant Shop =====" << std::endl;

    // Display all consumable items
    std::cout << "\n[Consumable Items]" << std::endl;
    for (int i = 0; i < 3; ++i) {
        goods.at(CONSUMABLE).at(i).displayItemInfo();
    }

    // Display all weapon items
    std::cout << "[Weapon Items]" << std::endl;
    for (int i = 0; i < 3; ++i) {
        goods.at(WEAPON).at(i).displayItemInfo();
    }

    // Display all armor items
    std::cout << "[Armor Items]" << std:: endl;
    for (int i = 0; i < 3; ++i) {
        goods.at(ARMOR).at(i).displayItemInfo();
    }
}

// Get merchant availability status
bool Merchant::getIsAvailable() const {
    return isAvailable;
}

// Set merchant availability status
void Merchant::setIsAvailable(bool state) {
    isAvailable = state;
}

// Get current game difficulty
int Merchant::getCurrentDiff() const {
    return currentDiff;
}
