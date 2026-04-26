#include "gametester.h"
#include "shop.h"
#include "merchant.h"
#include "item.h"
#include "player.h"
#include "gamelogger.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

GameTester::GameTester() : totalTests(0), passedTests(0) {}
GameTester::~GameTester() = default;

bool GameTester::inventoryHasItem(Player& p, const string& itemName) {
    const vector<string>& items = p.getInventory()->get_items();
    for (const string& name : items) {
        if (name == itemName) return true;
    }
    return false;
}

void GameTester::runAllTests() {
    cout << "\n=====================================\n";
    cout << "       SHOP SYSTEM FULL TEST\n";
    cout << "=====================================\n\n";

    testShopInitialization();
    testMerchantItemList();
    testBuyWithSufficientGold();
    testBuyWithInsufficientGold();
    testInventoryFullPurchase();
    testSellItem();
    testDifficultyBasedPricing();
    testTransactionLogWritten();

    cout << "\nAll tests executed.\n";
}

void GameTester::testShopInitialization() {
    totalTests++;
    Merchant m(1);
    Player p;
    Shop s;
    s.initShop(&m, &p, p.getInventory());

    if (s.get_isShopOpen()) {
        passedTests++;
        cout << "[PASS] Shop initialization\n";
    } else {
        recordBug("Shop failed to open");
        cout << "[FAIL] Shop initialization\n";
    }
}

void GameTester::testMerchantItemList() {
    totalTests++;
    Merchant m(1);
    bool ok = m.hasItem(CONSUMABLE, 0)
           && m.hasItem(WEAPON, 1)
           && m.hasItem(ARMOR, 2);

    if (ok) {
        passedTests++;
        cout << "[PASS] Merchant has all items\n";
    } else {
        recordBug("Merchant missing items");
        cout << "[FAIL] Merchant has all items\n";
    }
}

void GameTester::testBuyWithSufficientGold() {
    totalTests++;
    Merchant m(1);
    Player p;
    Shop s;
    s.initShop(&m, &p, p.getInventory());
    p.set_Money(1000);

    Item item = m.getItem(CONSUMABLE, 0);
    string name = item.getItemName();
    float before = p.get_Money();
    bool success = s.buyItem(CONSUMABLE, 0);

    if (success && p.get_Money() < before && inventoryHasItem(p, name)) {
        passedTests++;
        cout << "[PASS] Buy with enough gold\n";
    } else {
        recordBug("Buy failed with enough gold");
        cout << "[FAIL] Buy with enough gold\n";
    }
}

void GameTester::testBuyWithInsufficientGold() {
    totalTests++;
    Merchant m(1);
    Player p;
    Shop s;
    s.initShop(&m, &p, p.getInventory());
    p.set_Money(1);

    bool res = s.buyItem(CONSUMABLE, 2);
    if (!res) {
        passedTests++;
        cout << "[PASS] Buy with no gold (rejected)\n";
    } else {
        recordBug("Buy succeeded with no gold");
        cout << "[FAIL] Buy with no gold (rejected)\n";
    }
}

void GameTester::testInventoryFullPurchase() {
    totalTests++;
    Merchant m(1);
    Player p;
    Shop s;
    s.initShop(&m, &p, p.getInventory());
    Inventory* inv = p.getInventory();

    for (int i = 0; i < inv->get_capacity(); ++i) {
        inv->add_item("Filler_" + to_string(i));
    }

    bool res = s.buyItem(WEAPON, 0);
    if (!res) {
        passedTests++;
        cout << "[PASS] Inventory full (cannot buy)\n";
    } else {
        recordBug("Purchased into full inventory");
        cout << "[FAIL] Inventory full (cannot buy)\n";
    }
}

void GameTester::testSellItem() {
    totalTests++;
    Merchant m(1);
    Player p;
    Shop s;
    s.initShop(&m, &p, p.getInventory());
    Item item = m.getItem(CONSUMABLE, 0);
    string name = item.getItemName();
    p.getInventory()->add_item(name);

    float before = p.get_Money();
    bool ok = s.sellItem(CONSUMABLE, 0);

    if (ok && p.get_Money() > before && !inventoryHasItem(p, name)) {
        passedTests++;
        cout << "[PASS] Sell item successfully\n";
    } else {
        recordBug("Sell failed");
        cout << "[FAIL] Sell item successfully\n";
    }
}

void GameTester::testDifficultyBasedPricing() {
    totalTests++;

    // Normal
    Player pN; Merchant mN(1); Shop sN;
    sN.initShop(&mN, &pN, pN.getInventory());
    pN.set_Money(1000);
    sN.buyItem(WEAPON, 0);
    int costN = 1000 - pN.get_Money();

    // Easy
    Player pE; Merchant mE(0); Shop sE;
    sE.initShop(&mE, &pE, pE.getInventory());
    pE.set_Money(1000);
    sE.buyItem(WEAPON, 0);
    int costE = 1000 - pE.get_Money();

    // Hard
    Player pH; Merchant mH(2); Shop sH;
    sH.initShop(&mH, &pH, pH.getInventory());
    pH.set_Money(1000);
    sH.buyItem(WEAPON, 0);
    int costH = 1000 - pH.get_Money();

    bool valid = (costE <= costN) && (costH >= costN);
    if (valid) {
        passedTests++;
        cout << "[PASS] Difficulty-based pricing\n";
    } else {
        recordBug("Difficulty price multiplier wrong");
        cout << "[FAIL] Difficulty-based pricing\n";
    }
}

void GameTester::testTransactionLogWritten() {
    totalTests++;

    ofstream clearLog(LOG_FILE, ios::trunc);
    clearLog.close();

    GameLogger logger;
    logger.initLogFile();
    logger.logTransaction("TEST_BUY", CONSUMABLE, 0, 10);
    logger.closeLogFile();

    ifstream logFile(LOG_FILE);
    string content((istreambuf_iterator<char>(logFile)), istreambuf_iterator<char>());
    logFile.close();

    if (content.find("TEST_BUY") != string::npos) {
        passedTests++;
        cout << "[PASS] Transaction log written to file\n";
    } else {
        recordBug("Log file not written");
        cout << "[FAIL] Transaction log written to file\n";
    }
}

void GameTester::recordBug(const string& desc) {
    bugs.push_back(desc);
}

void GameTester::generateTestReport() const {
    cout << "\n=====================================\n";
    cout << "          TEST REPORT\n";
    cout << "=====================================\n";
    cout << "Total Tests: " << totalTests << "\n";
    cout << "Passed:      " << passedTests << "\n";
    cout << "Failed:      " << (totalTests - passedTests) << "\n\n";

    if (bugs.empty()) {
        cout << "All tests passed! No bugs found.\n";
    } else {
        cout << "Detected Bugs:\n";
        for (const string& bug : bugs) {
            cout << "- " << bug << "\n";
        }
    }
    cout << "=====================================\n\n";
}
