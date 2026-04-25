#include "shop.h"
#include "merchant.h"
#include "player.h"
#include "gamelogger.h"
#include "item.h"
#include <iostream>
#include <string>

// Constructor
Shop::Shop() {
    isShopOpen = false;
    sellDiscount = SELL_DISCOUNT;
    merchant = nullptr;
    player = nullptr;
    inventory = nullptr;
}

// Destructor
Shop::~Shop() {
    // No raw pointer deletion; owned by other modules
    merchant = nullptr;
    player = nullptr;
    inventory = nullptr;
}

// Initialize shop with merchant, player, inventory
void Shop::initShop(Merchant* m, Player* p, Inventory* inv) {
    this->merchant = m;
    this->player = p;
    this->inventory = inv;
    isShopOpen = true;
}

// Calculate selling price with discount
int Shop::calculateSellPrice(const Item& item) {
    int basePrice = item.getOriginalPurchasePrice();
    return static_cast<int>(basePrice * sellDiscount);
}

// Buy item from merchant (with difficulty-based price + logger + inventory check)
bool Shop::buyItem(ItemType type, int grade) {
    if (!isShopOpen || !merchant || !player || !inventory) {
        std::cout << "[Shop] Shop not initialized or closed." << std::endl;
        return false;
    }

    if (grade < 0 || grade > 2) {
        std::cout << "[Shop] Invalid grade." << std::endl;
        return false;
    }

    if (!merchant->hasItem(type, grade)) {
        std::cout << "[Shop] Item not available." << std::endl;
        return false;
    }

    // Get difficulty multiplier from merchant
    Difficulty diff = merchant->getCurrentDiff();
    float priceMultiplier = 1.0f;

    if (diff == Difficulty::EASY)      priceMultiplier = 0.8f;
    if (diff == Difficulty::NORMAL)    priceMultiplier = 1.0f;
    if (diff == Difficulty::HARD)      priceMultiplier = 1.3f;

    Item item = merchant->getItem(type, grade);
    int basePrice = item.getPrice();
    int finalPrice = static_cast<int>(basePrice * priceMultiplier);

    // Check player money
    if (player->get_Money() < finalPrice) {
        std::cout << "[Shop] Not enough gold." << std::endl;
        return false;
    }

    // Inventory capacity check
    if (inventory->get_current_size() >= inventory->get_capacity()) {
        std::cout << "[Shop] Inventory is full." << std::endl;
        return false;
    }

    // Deduct money
    player->change_Money(-finalPrice);
    item.setOriginalPurchasePrice(finalPrice);

    // Add item to inventory (Potion goes to inventory, NOT used immediately)
    std::string itemName = item.getName();
    inventory->add_item(itemName);

    // Log transaction
    GameLogger logger;
    logger.initLogFile();
    logger.logTransaction("BUY", type, grade, finalPrice);
    logger.closeLogFile();

    std::cout << "[Shop] Buy success: " << itemName << " Cost: " << finalPrice << " gold" << std::endl;
    return true;
}

// Sell item to merchant (with difficulty-based sell price + logger)
bool Shop::sellItem(ItemType type, int grade) {
    if (!isShopOpen || !merchant || !player || !inventory) {
        std::cout << "[Shop] Shop not initialized or closed." << std::endl;
        return false;
    }

    if (grade < 0 || grade > 2) {
        std::cout << "[Shop] Invalid grade." << std::endl;
        return false;
    }

    Item item = merchant->getItem(type, grade);
    std::string itemName = item.getName();

    // Check if inventory has the item
    bool hasItem = false;
    for (const std::string& name : inventory->get_items()) {
        if (name == itemName) {
            hasItem = true;
            break;
        }
    }

    if (!hasItem) {
        std::cout << "[Shop] Item not in inventory." << std::endl;
        return false;
    }

    // Difficulty sell multiplier
    Difficulty diff = merchant->getCurrentDiff();
    float sellMulti = 0.5f;

    if (diff == Difficulty::EASY)      sellMulti = 0.4f;
    if (diff == Difficulty::NORMAL)    sellMulti = 0.5f;
    if (diff == Difficulty::HARD)      sellMulti = 0.6f;

    int sellPrice = static_cast<int>(item.getPrice() * sellMulti);

    // Remove item
    inventory->remove_item(itemName);
    player->change_Money(sellPrice);

    // Log transaction
    GameLogger logger;
    logger.initLogFile();
    logger.logTransaction("SELL", type, grade, sellPrice);
    logger.closeLogFile();

    std::cout << "[Shop] Sell success: " << itemName << " Earned: " << sellPrice << " gold" << std::endl;
    return true;
}

// Show shop UI string
std::string Shop::showShopUI() {
    if (!merchant) return "Shop unavailable.";
    return merchant->showGoodsList();
}

// Close shop
void Shop::closeShop() {
    isShopOpen = false;
}

// Getters
bool Shop::get_isShopOpen() const {
    return isShopOpen;
}

float Shop::get_sellDiscount() const {
    return sellDiscount;
}
