#ifndef ITEM_H
#define ITEM_H

#include <string>

// Forward declaration
class Player;

// Item type enumeration
enum class ItemType {
    POTION,
    SWORD,
    ARMOR
};

// Difficulty enumeration (used by items)
enum class Difficulty {
    EASY,
    NORMAL,
    HARD
};

// Item class - represents a single item
class Item {
private:
    ItemType type;
    int grade;              // 0=Low, 1=Mid, 2=High
    std::string name;
    int effectValue;
    int price;
    int originalPurchasePrice;
    bool isConsumable;

public:
    Item();
    ~Item();

    // Initialization
    void initItem(ItemType type, int grade);

    // Getters
    ItemType getType() const;
    int getGrade() const;
    std::string getName() const;
    int getEffectValue() const;
    int getPrice() const;
    int getOriginalPurchasePrice() const;
    bool isConsumableItem() const;

    // Setters
    void setOriginalPurchasePrice(int price);

    // Item effects
    void applyEffect(Player& player);
};

// Global constants for items
const int ITEM_EFFECT_POTION[3] = { 5, 10, 15 };
const int ITEM_PRICE_POTION[3] = { 5, 10, 15 };

const int ITEM_EFFECT_SWORD[3] = { 10, 20, 30 };
const int ITEM_PRICE_SWORD[3] = { 15, 25, 40 };

const int ITEM_EFFECT_ARMOR[3] = { 5, 10, 15 };
const int ITEM_PRICE_ARMOR[3] = { 20, 35, 50 };

#endif // ITEM_H
