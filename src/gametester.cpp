#include "gametester.h"
#include "shop.h"
#include "merchant.h"
#include "item.h"
#include "player.h"
#include "gamelogger.h"
#include "types.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// Constructor
GameTester::GameTester() : totalTests(0), passedTests(0) {}

// Destructor
GameTester::~GameTester() = default;

// Helper: Reset player state before each test
void GameTester::resetPlayer(Player& p) {
    p.set_Money(1000);
    p.clearInventory(); // Assumes Inventory has clear method (per player.h structure)
}

// Helper: Check if inventory contains an item (by name)
bool GameTester::inventoryContains(const Inventory& inv, const std::string& itemName) {
    const auto& items = inv.get_items();
    for (const auto& name : items) {
        if (name == itemName) return true;
    }
    return false;
}

// Run all test cases for shop system
void GameTester::runAllTests() {
    std::cout << "\n=====================================" << std::endl;
    std::cout << "       Starting Shop System Test      " << std::endl;
    std::cout << "=====================================\n" << std::endl;

    // Initialize test modules
    Merchant merchant(Difficulty::NORMAL); // Use enum instead of magic number
    Player player;
    Shop shop;
    shop.initShop(&merchant, &player, &player.inventory); // Inventory is part of Player

    // Test cases
    testShopInitialization(shop);
    testItemAvailability(merchant);
    testPurchaseWithEnoughGold(shop, player, merchant);
    testPurchaseWithInsufficientGold(shop, player);
    testInventoryFull(shop, player, merchant);
    testSellItem(shop, player, merchant);
    testDifficultyPricing(player, merchant, shop);
    testTransactionLogging();

    std::cout << "\nAll tests completed.\n";
}

// Test shop initialization
void GameTester::testShopInitialization(Shop& shop) {
    totalTests++;
    bool result = shop.get_isShopOpen();
    if (result) {
        passedTests++;
        std::cout << "[PASS] Shop initialization test" << std::endl;
    } else {
        recordBug("Shop initialization failed: shop not open");
        std::cout << "[FAIL] Shop initialization test" << std::endl;
    }
}

// Test if merchant has expected items
void GameTester::testItemAvailability(Merchant& merchant) {
    totalTests++;
    bool hasLowConsumable = merchant.hasItem(CONSUMABLE, 0);
    bool hasMediumWeapon = merchant.hasItem(WEAPON, 1);
    bool hasHighArmor = merchant.hasItem(ARMOR, 2);
    if (hasLowConsumable && hasMediumWeapon && hasHighArmor) {
        passedTests++;
        std::cout << "[PASS] Item availability test" << std::endl;
    } else {
        recordBug("Merchant missing expected items");
        std::cout << "[FAIL] Item availability test" << std::endl;
    }
}

// Test successful purchase with enough gold
void GameTester::testPurchaseWithEnoughGold(Shop& shop, Player& p, Merchant& m) {
    resetPlayer(p);
    totalTests++;

    Item item = m.getItem(CONSUMABLE, 0);
    std::string itemName = item.getItemName();
    int expectedPrice = static_cast<int>(item.getPrice() * 1.0f); // Normal difficulty multiplier

    float initialGold = p.get_Money();
    bool purchaseSuccess = shop.buyItem(CONSUMABLE, 0);
    bool goldDeducted = (p.get_Money() == initialGold - expectedPrice);
    bool itemInInventory = inventoryContains(p.inventory, itemName);

    if (purchaseSuccess && goldDeducted && itemInInventory) {
        passedTests++;
        std::cout << "[PASS] Purchase with enough gold test" << std::endl;
    } else {
        recordBug("Purchase failed despite sufficient gold or state not updated correctly");
        std::cout << "[FAIL] Purchase with enough gold test" << std::endl;
    }
}

// Test purchase with insufficient gold
void GameTester::testPurchaseWithInsufficientGold(Shop& shop, Player& p) {
    resetPlayer(p);
    totalTests++;

    p.set_Money(0);
    bool purchaseSuccess = shop.buyItem(CONSUMABLE, 2); // High-priced item
    bool goldUnchanged = (p.get_Money() == 0);

    if (!purchaseSuccess && goldUnchanged) {
        passedTests++;
        std::cout << "[PASS] Purchase with insufficient gold test" << std::endl;
    } else {
        recordBug("Purchase succeeded with no gold or money deducted incorrectly");
        std::cout << "[FAIL] Purchase with insufficient gold test" << std::endl;
    }
}

// Test inventory full behavior
void GameTester::testInventoryFull(Shop& shop, Player& p, Merchant& m) {
    resetPlayer(p);
    totalTests++;

    // Fill inventory to capacity (Inventory is part of Player)
    for (int i = 0; i < p.inventory.get_capacity(); ++i) {
        p.inventory.add_item("DummyItem_" + std::to_string(i));
    }

    bool purchaseSuccess = shop.buyItem(WEAPON, 0); // Try to buy when full
    bool inventorySizeUnchanged = (p.inventory.get_current_size() == p.inventory.get_capacity());

    if (!purchaseSuccess && inventorySizeUnchanged) {
        passedTests++;
        std::cout << "[PASS] Inventory full test" << std::endl;
    } else {
        recordBug("Item added to full inventory or purchase succeeded incorrectly");
        std::cout << "[FAIL] Inventory full test" << std::endl;
    }
}

// Test item selling
void GameTester::testSellItem(Shop& shop, Player& p, Merchant& m) {
    resetPlayer(p);
    totalTests++;

    // Prepare item to sell
    Item item = m.getItem(CONSUMABLE, 0);
    std::string itemName = item.getItemName();
    p.inventory.add_item(itemName);

    float initialGold = p.get_Money();
    bool sellSuccess = shop.sellItem(CONSUMABLE, 0);
    int expectedSellPrice = static_cast<int>(item.getPrice() * 0.5f); // Normal difficulty multiplier
    bool goldIncreased = (p.get_Money() == initialGold + expectedSellPrice);
    bool itemRemoved = !inventoryContains(p.inventory, itemName);

    if (sellSuccess && goldIncreased && itemRemoved) {
        passedTests++;
        std::cout << "[PASS] Item selling test" << std::endl;
    } else {
        recordBug("Sell failed, gold not updated, or item not removed");
        std::cout << "[FAIL] Item selling test" << std::endl;
    }
}

// Test difficulty-based pricing logic
void GameTester::testDifficultyPricing(Player& p, Merchant& m, Shop& shop) {
    resetPlayer(p);
    totalTests++;

    // Test different difficulty multipliers
    bool testPassed = true;

    // Test EASY mode
    Merchant merchantEasy(Difficulty::EASY);
    Shop shopEasy;
    shopEasy.initShop(&merchantEasy, &p, &p.inventory);
    p.set_Money(1000);
    Item easyItem = merchantEasy.getItem(WEAPON, 1);
    int easyPrice = static_cast<int>(easyItem.getPrice() * 0.8f);
    shopEasy.buyItem(WEAPON, 1);
    if (p.get_Money() != 1000 - easyPrice) testPassed = false;

    // Test HARD mode
    Merchant merchantHard(Difficulty::HARD);
    Shop shopHard;
    shopHard.initShop(&merchantHard, &p, &p.inventory);
    p.set_Money(1000);
    Item hardItem = merchantHard.getItem(WEAPON, 1);
    int hardPrice = static_cast<int>(hardItem.getPrice() * 1.3f);
    shopHard.buyItem(WEAPON, 1);
    if (p.get_Money() != 1000 - hardPrice) testPassed = false;

    if (testPassed) {
        passedTests++;
        std::cout << "[PASS] Difficulty-based pricing test" << std::endl;
    } else {
        recordBug("Difficulty price multipliers not applied correctly");
        std::cout << "[FAIL] Difficulty-based pricing test" << std::endl;
    }
}

// Test transaction logging (with file verification)
void GameTester::testTransactionLogging() {
    totalTests++;

    // Clear previous test log
    std::ofstream clearLog(LOG_FILE, std::ios::trunc);
    clearLog.close();

    // Perform a test transaction
    GameLogger logger;
    logger.initLogFile();
    logger.logTransaction("TEST_BUY", CONSUMABLE, 0, 10);
    logger.closeLogFile();

    // Verify log was written
    std::ifstream logFile(LOG_FILE);
    std::string logContent((std::istreambuf_iterator<char>(logFile)), std::istreambuf_iterator<char>());
    logFile.close();

    if (logContent.find("TEST_BUY") != std::string::npos) {
        passedTests++;
        std::cout << "[PASS] Transaction logging test" << std::endl;
    } else {
        recordBug("Transaction log not written correctly");
        std::cout << "[FAIL] Transaction logging test" << std::endl;
    }
}

// Record a bug
void GameTester::recordBug(const std::string& bugDesc) {
    bugs.push_back(bugDesc);
}

// Generate final test report
void GameTester::generateTestReport() const {
    std::cout << "\n=====================================" << std::endl;
    std::cout << "          Test Report                 " << std::endl;
    std::cout << "=====================================\n" << std::endl;

    std::cout << "Total Tests: " << totalTests << std::endl;
    std::cout << "Passed: " << passedTests << std::endl;
    std::cout << "Failed: " << (totalTests - passedTests) << "\n" << std::endl;

    if (bugs.empty()) {
        std::cout << "No bugs detected.\n";
    } else {
        std::cout << "Detected Bugs:\n";
        for (size_t i = 0; i < bugs.size(); ++i) {
            std::cout << "- " << bugs[i] << std::endl;
        }
    }
    std::cout << "\n";
}
