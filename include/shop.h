#ifndef SHOP_H
#define SHOP_H

#include "item.h"
#include <string>

// Forward declarations
class Merchant;
class Player;
class Inventory;

// Shop class - handles transaction logic
class Shop {
private:
    Merchant* merchant;
    Player* player;
    Inventory* inventory;
    bool isShopOpen;
    float sellDiscount;

public:
    Shop();
    ~Shop();

    // Initialization
    void initShop(Merchant* m, Player* p);

    // Transactions
    bool buyItem(ItemType type, int grade);
    bool sellItem(ItemType type, int grade);

    // Pricing
    int calculateSellPrice(const Item& item);

    // Shop UI
    std::string showShopUI();
    void closeShop();

    // Getters
    bool get_isShopOpen() const;
    float get_sellDiscount() const;
};

// Global constants
const float SELL_DISCOUNT = 0.5f;

#endif // SHOP_H
