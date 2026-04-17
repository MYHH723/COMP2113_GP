#ifndef MERCHANT_H
#define MERCHANT_H

#include "item.h"
#include <map>
#include <array>
#include <string>

// Merchant class - manages goods display and stock
class Merchant {
private:
    std::map<ItemType, std::array<Item, 3>> goods;  // 3 grades per item type
    int maxStockPerItem;
    bool isAvailable;
    Difficulty currentDiff;

public:
    Merchant();
    ~Merchant();

    // Initialization
    void initMerchant(Difficulty diff);

    // Goods management
    bool hasItem(ItemType type, int grade);
    Item getItem(ItemType type, int grade);
    std::string showGoodsList();

    // Availability
    bool getIsAvailable() const;
    void setIsAvailable(bool status);

    // Getters
    Difficulty getCurrentDiff() const;
};

// Global constants
const int MAX_STOCK = 999;

#endif // MERCHANT_H
