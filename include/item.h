#ifndef ITEM_H
#define ITEM_H

#include <string>

// Forward declaration
class Player;

// Item class - represents a single item
class Item {
private:
    std::string name;
    ItemType type;
    ItemRarity rarity; 
    float effectValue; // For potions: heal amount; for weapons: attack boost; for armor: defense boost
    int price;
    bool isConsumed;

public:
    Item();
    Item(const std::string name, ItemType type, ItemRarity rarity,
           int cost, float effect, bool consumed = false);
    ~Item();

    // Initialization
    void initItem(ItemType type, int grade);

    // Getters
    ItemType getType() const;
    ItemRarity getRarity() const;
    std::string getName() const;
    float getEffectValue() const;
    int getPrice() const;
    bool getIsConsumed() const;
    void setIsConsumed(bool state);


    // Item effects
    void applyEffect(Player& player);
    void displayItemInfo() const;
};

// Global constants for items
const int ITEM_EFFECT_POTION[3] = { 5, 10, 15 };
const int ITEM_PRICE_POTION[3] = { 5, 10, 15 };

const int ITEM_EFFECT_SWORD[3] = { 10, 20, 30 };
const int ITEM_PRICE_SWORD[3] = { 15, 25, 40 };

const int ITEM_EFFECT_ARMOR[3] = { 5, 10, 15 };
const int ITEM_PRICE_ARMOR[3] = { 20, 35, 50 };

#endif // ITEM_H
