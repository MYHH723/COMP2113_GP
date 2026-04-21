#include "player.h" 
#include "savegame.h"
#include <iostream>
#include <fstream>
#include <algorithm>

// Inventory implementation
Inventory::Inventory() : capacity(MAX_INVENTORY_SIZE) {}

Inventory::~Inventory() {}

bool Inventory::add_item(const std::string& itemName) {
    if (items.size() < capacity) {
        items.push_back(itemName);
        return true;
    }
    return false;
}

bool Inventory::remove_item(const std::string& itemName) {
    auto it = std::find(items.begin(), items.end(), itemName);
    if (it != items.end()) {
        items.erase(it);
        return true;
    }
    return false;
}

bool Inventory::use_item(const std::string& itemName) {
    // Simple implementation: remove on use
    return remove_item(itemName);
}

void Inventory::show_items() const {
    std::cout << "Inventory (" << items.size() << "/" << capacity << "):" << std::endl;
    for (const auto& itemStr : items) {
        // Parse name from "Type:Grade:Name:..."
        size_t pos1 = itemStr.find(':');
        if (pos1 != std::string::npos) {
            size_t pos2 = itemStr.find(':', pos1 + 1);
            if (pos2 != std::string::npos) {
                size_t pos3 = itemStr.find(':', pos2 + 1);
                if (pos3 != std::string::npos) {
                    std::string name = itemStr.substr(pos2 + 1, pos3 - pos2 - 1);
                    std::cout << "- " << name << std::endl;
                }
            }
        }
    }
}

void Inventory::sort_items() {
    // Define type priorities: Sword > Armor > Potion
    std::map<std::string, int> typePriority = {
        {"SWORD", 3},
        {"ARMOR", 2},
        {"POTION", 1}
    };

    items.sort([&typePriority](const std::string& a, const std::string& b) {
        // Parse from "Type:Grade:Name:..."
        auto parse = [](const std::string& s) -> std::pair<std::string, int> {
            size_t pos1 = s.find(':');
            if (pos1 == std::string::npos) return {"", 0};
            std::string type = s.substr(0, pos1);
            size_t pos2 = s.find(':', pos1 + 1);
            if (pos2 == std::string::npos) return {type, 0};
            int grade = std::stoi(s.substr(pos1 + 1, pos2 - pos1 - 1));
            return {type, grade};
        };

        auto [typeA, gradeA] = parse(a);
        auto [typeB, gradeB] = parse(b);

        // First compare by type priority (higher first)
        int priA = typePriority.count(typeA) ? typePriority[typeA] : 0;
        int priB = typePriority.count(typeB) ? typePriority[typeB] : 0;
        if (priA != priB) {
            return priA > priB;
        }
        // Then by grade (higher first)
        return gradeA > gradeB;
    });
}

int Inventory::get_capacity() const {
    return capacity;
}

int Inventory::get_current_size() const {
    return items.size();
}

// Panel implementation
Panel::Panel() {}

Panel::~Panel() {}

void Panel::show_status(const Player& player) {
    std::cout << "=== Player Status ===" << std::endl;
    std::cout << "ATK: " << player.get_ATK() << std::endl;
    std::cout << "DEF: " << player.get_DEF() << std::endl;
    std::cout << "HP: " << player.get_HP() << std::endl;
    std::cout << "EXP: " << player.get_EXP() << std::endl;
    std::cout << "Money: " << player.get_Money() << std::endl;
    std::cout << "Alive: " << (player.get_isAlive() ? "Yes" : "No") << std::endl;
    std::cout << "Poisoned: " << (player.get_isPoisoned() ? "Yes" : "No") << std::endl;
    std::cout << "Stunned: " << (player.get_isStunned() ? "Yes" : "No") << std::endl;
}

void Panel::show_inventory(const Player& player) {
    std::cout << "=== Inventory ===" << std::endl;
    player.show_items();
}

void Panel::show_condition(const Player& player) {
    std::cout << "=== Condition ===" << std::endl;
    std::cout << "Alive: " << (player.get_isAlive() ? "Yes" : "No") << std::endl;
    std::cout << "Poisoned: " << (player.get_isPoisoned() ? "Yes" : "No") << std::endl;
    std::cout << "Stunned: " << (player.get_isStunned() ? "Yes" : "No") << std::endl;
}

void Panel::show_message(const std::string& message) {
    std::cout << "Message: " << message << std::endl;
}

// Player implementation
Player::Player() {
    state["ATK"] = DEFAULT_ATK;
    state["DEF"] = DEFAULT_DEF;
    state["HP"] = DEFAULT_HP;
    state["EXP"] = DEFAULT_EXP;
    state["Money"] = DEFAULT_MONEY;
    inventory = new Inventory();
    isAlive = true;
    isPoisoned = false;
    isStunned = false;
}

Player::~Player() {
    delete inventory;
}

std::map<std::string, float> Player::get_state() const {
    return state;
}

float Player::get_ATK() const {
    return state.at("ATK");
}

float Player::get_DEF() const {
    return state.at("DEF");
}

float Player::get_HP() const {
    return state.at("HP");
}

float Player::get_EXP() const {
    return state.at("EXP");
}

float Player::get_Money() const {
    return state.at("Money");
}

bool Player::get_isAlive() const {
    return isAlive;
}

bool Player::get_isPoisoned() const {
    return isPoisoned;
}

bool Player::get_isStunned() const {
    return isStunned;
}

void Player::change_state(const std::string& key, float value) {
    state[key] = value;
}

void Player::change_ATK(float amount) {
    state["ATK"] += amount;
}

void Player::change_DEF(float amount) {
    state["DEF"] += amount;
}

void Player::change_HP(float amount) {
    state["HP"] += amount;
    if (state["HP"] <= 0) {
        isAlive = false;
    }
}

void Player::change_EXP(float amount) {
    state["EXP"] += amount;
}

void Player::change_Money(float amount) {
    state["Money"] += amount;
}

void Player::set_isAlive(bool alive) {
    isAlive = alive;
}

void Player::set_isPoisoned(bool poisoned) {
    isPoisoned = poisoned;
}

void Player::set_isStunned(bool stunned) {
    isStunned = stunned;
}

void Player::add_item(const std::string& itemName) {
    inventory->add_item(itemName);
}

void Player::remove_item(const std::string& itemName) {
    inventory->remove_item(itemName);
}

void Player::use_item(const std::string& itemName) {
    inventory->use_item(itemName);
}

void Player::show_items() const {
    inventory->show_items();
}

void Player::sort_items() {
    inventory->sort_items();
}

std::string Player::itemToString(const Item& item) {
    std::string typeStr;
    switch (item.getType()) {
        case ItemType::POTION: typeStr = "POTION"; break;
        case ItemType::SWORD: typeStr = "SWORD"; break;
        case ItemType::ARMOR: typeStr = "ARMOR"; break;
    }
    return typeStr + ":" + std::to_string(item.getGrade()) + ":" + item.getName() + ":" +
           std::to_string(item.getEffectValue()) + ":" + std::to_string(item.getPrice()) + ":" +
           std::to_string(item.getOriginalPurchasePrice()) + ":" + (item.isConsumableItem() ? "true" : "false");
}

void Player::save() {
    saveGame(*this);  // Delegate to modular implementation
}

void Player::load_save() {
    loadGame(*this);  // Delegate to modular implementation
}

// Demo main function
int main() {
    Player player;
    Panel panel;

    // Show initial status
    panel.show_status(player);
    panel.show_inventory(player);
    panel.show_condition(player);

    // Modify player
    player.change_ATK(50);
    player.change_HP(-200);

    Item sword2;
    sword2.initItem(ItemType::SWORD, 2);  // Level3
    player.add_item(player.itemToString(sword2));

    Item armor1;
    armor1.initItem(ItemType::ARMOR, 1);  // Level2
    player.add_item(player.itemToString(armor1));

    Item sword0;
    sword0.initItem(ItemType::SWORD, 0);  // Level1
    player.add_item(player.itemToString(sword0));

    panel.show_message("Player gained 50 ATK, lost 200 HP, and acquired items.");

    // Show updated status
    panel.show_status(player);
    panel.show_inventory(player);

    // Sort items
    player.sort_items();
    panel.show_message("Items sorted.");
    panel.show_inventory(player);

    // Save and load
    player.save();
    player.change_Money(100);
    panel.show_status(player);
    player.load_save();
    panel.show_message("Loaded save.");
    panel.show_status(player);

    return 0;
}

