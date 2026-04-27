#include "item.h"
#include "types.h"
#include <iostream>
#include <string>
#include <iomanip>

// Default constructor
Item::Item()
    : name(""), type(POTION), rarity(LOW),
      price(0), effectValue(0.0),
      isConsumed(false) {}

// Parameterized constructor
Item::Item(const std::string name, ItemType type, ItemRarity rarity,
           int cost, float effect, bool consumed)
    : name(name), type(type), rarity(rarity),
      price(cost), effectValue(effect),
      isConsumed(consumed) {}

// Get name of item
std::string Item::getName() const {
    return name;
}

// Get item type (CONSUMABLE, WEAPON, ARMOR)
ItemType Item::getType() const {
    return type;
}

// Get item rarity (LOW, MEDIUM, HIGH)
ItemRarity Item::getRarity() const {
    return rarity;
}

// Get item price
int Item::getPrice() const {
    return price;
}

float Item::getEffectValue() const {
    return effectValue;
}

// Check if item is consumed
bool Item::getIsConsumed() const {
    return isConsumed;
}

// Set consume state
void Item::setIsConsumed(bool state) {
    isConsumed = state;
}

// Display item information
void Item::displayItemInfo() const {
    std::cout << std::left;
    std::cout << "Item: " + name;
    std::cout << std::setw(20) << "Type: ";

    // Display type
    switch (type) {
        case POTION: std::cout << "POTION"; break;
        case WEAPON: std::cout << "WEAPON"; break;
        case ARMOR: std::cout << "ARMOR"; break;
        default: std::cout << "Unknown"; break;
    }

    std::cout << std::setw(35) <<"Rarity: ";

    // Display rarity
    switch (rarity) {
        case LOW: std::cout << "Low"; break;
        case MEDIUM: std::cout << "Medium"; break;
        case HIGH: std::cout << "High"; break;
        default: std::cout << "Unknown"; break;
    }

    std::cout << std::setw(50) << "Price: " + std::to_string(price) << std::setw(60);
    switch (type) {
        case POTION: std::cout << "Heal: " << effectValue << "\n"; break;
        case WEAPON: std::cout << "Attack: " << effectValue << "\n"; break;
        case ARMOR: std::cout << "Defense: " << effectValue << "\n"; break;
        default: std::cout << "Unknown"; break;
    }
    
}

bool Item::operator==(const Item& other) const {
    return name == other.name &&
           type == other.type &&
           rarity == other.rarity &&
           price == other.price &&
           effectValue == other.effectValue;
}
