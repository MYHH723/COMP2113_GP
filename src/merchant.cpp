#include "merchant.h"
#include "item.h"
#include "types.h"
#include <iostream>
#include <random>
#include <string>

std::mt19937 gen(std::random_device{}());
std::uniform_int_distribution<> int_dist(0, 30);

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
    goods[POTION] = std::array<Item, 3>();
    goods[POTION][0] = Item("Low Potion", ItemType::POTION, ItemRarity::LOW, (int_dist(gen) + 10)/2, (int_dist(gen) + 50)*0.2);
    goods[POTION][1] = Item("Medium Potion", ItemType::POTION, ItemRarity::MEDIUM, (int_dist(gen) + 50)/2, (int_dist(gen) + 100)*0.5);
    goods[POTION][2] = Item("High Potion", ItemType::POTION, ItemRarity::HIGH, (int_dist(gen) + 100)/2, (int_dist(gen) + 200));

    // Initialize Weapon Items (Sword)
    goods[WEAPON] = std::array<Item, 3>();
    goods[WEAPON][0] = Item("Iron Sword", ItemType::WEAPON, ItemRarity::LOW, (int_dist(gen) + 10), (int_dist(gen) + 10)*0.1);
    goods[WEAPON][1] = Item("Steel Sword", ItemType::WEAPON, ItemRarity::MEDIUM, (int_dist(gen) + 50), (int_dist(gen) + 50)*0.15);
    goods[WEAPON][2] = Item("Golden Sword", ItemType::WEAPON, ItemRarity::HIGH, (int_dist(gen) + 100), (int_dist(gen) + 100)*0.2);

    // Initialize Armor Items
    goods[ARMOR] = std::array<Item, 3>();
    goods[ARMOR][0] = Item("Cloth Armor", ItemType::ARMOR, ItemRarity::LOW, (int_dist(gen) + 10), (int_dist(gen) + 10)*0.1);
    goods[ARMOR][1] = Item("Iron Armor", ItemType::ARMOR, ItemRarity::MEDIUM, (int_dist(gen) + 50), (int_dist(gen) + 50)*0.15);
    goods[ARMOR][2] = Item("Dragon Armor", ItemType::ARMOR, ItemRarity::HIGH, (int_dist(gen) + 100), (int_dist(gen) + 100)*0.2);
}

// Check if the merchant has the specified item type and index
bool Merchant::hasItem(ItemType type, ItemRarity rarity) const {
    if(goods.at(type).at(static_cast<int>(rarity)) == Item()) {
        return false;
    }
    return true;
}

// Get the item from goods by type and index
Item Merchant::getItem(ItemType type, ItemRarity rarity){
    if (hasItem(type, rarity)) {
        Item item = goods.at(type).at(static_cast<int>(rarity));
        goods.at(type).at(static_cast<int>(rarity)) = Item(); // Remove item from stock
        return item;
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
        goods.at(POTION).at(i).displayItemInfo();
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

void Merchant::restockGoods() {
    initMerchant();
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
