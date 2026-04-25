#include "item.h"
#include <iostream>
#include <string>

// Default constructor
Item::Item()
    : itemName(""), itemType(CONSUMABLE), itemRarity(LOW),
      price(0), healAmount(0), attackBoost(0), defenseBoost(0),
      isConsumed(false) {}

// Parameterized constructor
Item::Item(const std::string& name, ItemType type, ItemRarity rarity,
           int cost, int heal, int attack, int defense)
    : itemName(name), itemType(type), itemRarity(rarity),
      price(cost), healAmount(heal), attackBoost(attack), defenseBoost(defense),
      isConsumed(false) {}

// Get name of item
std::string Item::getItemName() const {
    return itemName;
}

// Get item type (CONSUMABLE, WEAPON, ARMOR)
ItemType Item::getItemType() const {
    return itemType;
}

// Get item rarity (LOW, MEDIUM, HIGH)
ItemRarity Item::getItemRarity() const {
    return itemRarity;
}

// Get item price
int Item::getPrice() const {
    return price;
}

// Get heal amount
int Item::getHealAmount() const {
    return healAmount;
}

// Get attack boost value
int Item::getAttackBoost() const {
    return attackBoost;
}

// Get defense boost value
int Item::getDefenseBoost() const {
    return defenseBoost;
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
    std::cout << "Item: " << itemName << "\n";
    std::cout << "Type: ";

    // Display type
    switch (itemType) {
        case CONSUMABLE: std::cout << "Consumable"; break;
        case WEAPON: std::cout << "Weapon"; break;
        case ARMOR: std::cout << "Armor"; break;
        default: std::cout << "Unknown"; break;
    }

    std::cout << "\nRarity: ";

    // Display rarity
    switch (itemRarity) {
        case LOW: std::cout << "Low"; break;
        case MEDIUM: std::cout << "Medium"; break;
        case HIGH: std::cout << "High"; break;
        default: std::cout << "Unknown"; break;
    }

    std::cout << "\nPrice: " << price << " gold\n";
    std::cout << "Heal: " << healAmount << "\n";
    std::cout << "Attack: " << attackBoost << "\n";
    std::cout << "Defense: " << defenseBoost << "\n\n";
}
