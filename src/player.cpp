#include "player.h"
#include "savegame.h"
#include "item.h"
#include "types.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// Inventory implementation
Inventory::Inventory() : capacity(MAX_INVENTORY_SIZE) {}

Inventory::~Inventory() {}

bool Inventory::add_item(const std::string &itemName)
{
    if (items.size() < capacity)
    {
        items.push_back(itemName);
        sort_items();
        return true;
    }
    return false;
}

bool Inventory::remove_item(const std::string &itemName)
{
    auto it = std::find(items.begin(), items.end(), itemName);
    if (it != items.end())
    {
        items.erase(it);
        return true;
    }
    return false;
}

std::string Inventory::get_item(const std::string &itemName) {
    auto it = std::find(items.begin(), items.end(), itemName);
    if (it != items.end()) {
        return *it;
    }
    return "";
}

bool Inventory::use_item(const std::string &itemName)
{
    // Simple implementation: remove on use
    return remove_item(itemName);
}

void Inventory::sort_items()
{
    // Define type priorities: WEAPON > ARMOR > POTION
    std::map<std::string, int> typePriority = {
        {"WEAPON", 3},
        {"ARMOR", 2},
        {"POTION", 1}};

    items.sort([&typePriority](const std::string &a, const std::string &b)
               {
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
        return gradeA > gradeB; });
}

int Inventory::get_capacity() const
{
    return capacity;
}

int Inventory::get_current_size() const
{
    return items.size();
}



// Player implementation
Player::Player(std::string name) : playerName(name)
{
    state["LEVEL"] = 1;
    state["ATK"] = DEFAULT_ATK;
    state["DEF"] = DEFAULT_DEF;
    state["HP"] = DEFAULT_HP;
    state["EXP"] = DEFAULT_EXP;
    state["Money"] = DEFAULT_MONEY;
    inventory = new Inventory();
    maxHP = DEFAULT_HP;
    equippedItems["WEAPON"] = "";
    equippedItems["ARMOR"] = "";
    isAlive = true;
    isPoisoned = false;
    isStunned = false;
}

Player::~Player()
{
    delete inventory;
}

std::map<std::string, float> Player::get_state() const
{
    return state;
}

int Player::get_Level() const
{
    return static_cast<int>(state.at("LEVEL"));
}

float Player::get_ATK() const
{
    return state.at("ATK");
}

float Player::get_DEF() const
{
    return state.at("DEF");
}

float Player::get_HP() const
{
    return state.at("HP");
}

float Player::get_EXP() const
{
    return state.at("EXP");
}

float Player::get_Money() const
{
    return state.at("Money");
}

float Player::get_maxHP() const
{
    return maxHP;
}

bool Player::get_isAlive() const
{
    return isAlive;
}

bool Player::get_isPoisoned() const
{
    return isPoisoned;
}

bool Player::get_isStunned() const
{
    return isStunned;
}

std::list<std::string> Player::get_all_items() const{
    return inventory->get_items();
}

void Player::change_state(const std::string &key, float value)
{
    state[key] = value;
}

void Player::change_ATK(float amount)
{
    state["ATK"] += amount;
}

void Player::change_DEF(float amount)
{
    state["DEF"] += amount;
}

void Player::change_HP(float amount)
{
    state["HP"] += amount;
    if (state["HP"] > maxHP)
    {
        state["HP"] = maxHP;
    }
    if (state["HP"] <= 0)
    {
        isAlive = false;
    }
}

void Player::change_EXP(float amount)
{
    state["EXP"] += amount;
    int level = state.at("LEVEL");
    float exp = state.at("EXP");
    while(exp >= 100 * std::pow(1.1, level - 1)) {
        exp -= 100 * std::pow(1.1, level - 1); 
        level++;
        level_up();
    }
    state["LEVEL"] = level;
    state["EXP"] = exp;
}

void Player::change_Money(float amount)
{
    state["Money"] += amount;
}

void Player::set_isAlive(bool alive)
{
    isAlive = alive;
}

void Player::set_isPoisoned(bool poisoned)
{
    isPoisoned = poisoned;
}

void Player::set_isStunned(bool stunned)
{
    isStunned = stunned;
}

void Player::add_item(const std::string &itemName)
{
    inventory->add_item(itemName);
}

void Player::remove_item(const std::string &itemName)
{
    inventory->remove_item(itemName);
}

void Player::use_item(const std::string &itemName)
{
    inventory->use_item(itemName);
}

void Player::sort_items()
{
    inventory->sort_items();
}

void Player::level_up()
{
    std::cout << "Level Up! You are now level " << get_Level() << "!" << std::endl;
    for(int time = get_Level() + 1; time > 0; time--) {
        std::cout << "Choose an attribute to increase: " << "(remaining " << time << " points)" << std::endl;
        std::cout << "1. ATK" << std::endl;
        std::cout << "2. DEF" << std::endl;
        std::cout << "3. HP" << std::endl;

        std::string choice;
        while(std::cin >> choice) {
            if (choice == "1" || choice == "ATK" || choice == "2" || choice == "DEF" || choice == "3" || choice == "HP") {
                break;
            }
            std::cout << "Invalid choice. Please enter 1, 2, or 3." << std::endl;
        }

        if (choice == "1" || choice == "ATK") {
            change_ATK(0.1*DEFAULT_ATK);
        }
        else if (choice == "2" || choice == "DEF") {
            change_DEF(0.1*DEFAULT_DEF);
        }
        else if (choice == "3" || choice == "HP") {
            maxHP += 0.1*DEFAULT_HP;
            change_HP(0.1*DEFAULT_HP);
        }
    }
    
}

void Player::equip_weapon(const std::string &itemName)
{
    std::string itemStr = inventory->get_item(itemName);
    if(inventory->remove_item(itemName)) {
        if (!equippedItems["WEAPON"].empty()) {
            float oldEffect = parseEffectValue(equippedItems["WEAPON"]);
            state["ATK"] -= oldEffect;
            inventory->add_item(equippedItems["WEAPON"]);
        }
        equippedItems["WEAPON"] = itemStr;
        float newEffect = parseEffectValue(itemStr);
        state["ATK"] += newEffect;
    }

}

void Player::equip_armor(const std::string &itemName)
{
    std::string itemStr = inventory->get_item(itemName);
    if(inventory->remove_item(itemName)) {
        if (!equippedItems["ARMOR"].empty()) {
            float oldEffect = parseEffectValue(equippedItems["ARMOR"]);
            state["DEF"] -= oldEffect;
            inventory->add_item(equippedItems["ARMOR"]);
        }
        equippedItems["ARMOR"] = itemStr;
        float newEffect = parseEffectValue(itemStr);
        state["DEF"] += newEffect;
    }

}


std::string Player::itemToString(const Item &item)
{
    std::string typeStr;
    switch (item.getType())
    {
    case ItemType::POTION:
        typeStr = "POTION";
        break;
    case ItemType::WEAPON:
        typeStr = "WEAPON";
        break;
    case ItemType::ARMOR:
        typeStr = "ARMOR";
        break;
    }
    return typeStr + ":" + std::to_string(item.getRarity()) + ":" + item.getName() + ":" +
           std::to_string(item.getEffectValue()) + ":" + std::to_string(item.getPrice());
}

float Player::parseEffectValue(const std::string& itemStr) const {

    size_t pos1 = itemStr.find(':');
    size_t pos2 = itemStr.find(':', pos1 + 1);
    size_t pos3 = itemStr.find(':', pos2 + 1);
    size_t pos4 = itemStr.find(':', pos3 + 1);
    
    std::string effectStr = itemStr.substr(pos3 + 1, pos4 - pos3 - 1);
    return std::stof(effectStr);
}

json Player::toJson() const {
    json j;
    
    // === Stats (from state map) ===
    j["stats"]["LEVEL"] = get_Level();
    j["stats"]["ATK"] = get_ATK();
    j["stats"]["DEF"] = get_DEF();
    j["stats"]["HP"] = get_HP();
    j["stats"]["EXP"] = get_EXP();
    j["stats"]["Money"] = get_Money();
    j["stats"]["maxHP"] = maxHP;
    
    // === Status flags ===
    j["status"]["isAlive"] = isAlive;
    j["status"]["isPoisoned"] = isPoisoned;
    j["status"]["isStunned"] = isStunned;
    
    // === Inventory (preserve your exact string format) ===
    j["inventory"] = json::array();
    for (const auto& itemStr : inventory->get_items()) {
        j["inventory"].push_back(itemStr);  // "SWORD:2:Iron Blade:25:150:150:false"
    }

    j["equipped"]["WEAPON"] = equippedItems.at("WEAPON");
    j["equipped"]["ARMOR"] = equippedItems.at("ARMOR");
    
    return j;
}

void Player::fromJson(const json& j) {
    // === Load stats ===
    if (j.contains("stats")) {
        const auto& stats = j["stats"];
        if (stats.contains("LEVEL")) state["LEVEL"] = stats["LEVEL"].get<float>();
        if (stats.contains("ATK")) state["ATK"] = stats["ATK"].get<float>();
        if (stats.contains("DEF")) state["DEF"] = stats["DEF"].get<float>();
        if (stats.contains("HP")) {
            state["HP"] = stats["HP"].get<float>();
            if (state["HP"] <= 0) isAlive = false;  // Sync alive flag
        }
        if (stats.contains("EXP")) state["EXP"] = stats["EXP"].get<float>();
        if (stats.contains("Money")) state["Money"] = stats["Money"].get<float>();
        if (stats.contains("maxHP")) maxHP = stats["maxHP"].get<float>();
    }
    
    // === Load status flags ===
    if (j.contains("status")) {
        const auto& status = j["status"];
        if (status.contains("isAlive")) isAlive = status["isAlive"].get<bool>();
        if (status.contains("isPoisoned")) isPoisoned = status["isPoisoned"].get<bool>();
        if (status.contains("isStunned")) isStunned = status["isStunned"].get<bool>();
    }
    
    // === Load inventory (clear first, then restore) ===
    if (j.contains("inventory") && j["inventory"].is_array()) {
        inventory->clear_items();  
        for (const auto& itemStr : j["inventory"]) {
            inventory->add_item(itemStr.get<std::string>());  
        }
    }
    
    // === Load equipped items ===
    if (j.contains("equipped")) {
        const auto& equipped = j["equipped"];
        if (equipped.contains("WEAPON")) {
            equippedItems["WEAPON"] = equipped["WEAPON"].get<std::string>();
        }
        if (equipped.contains("ARMOR")) {
            equippedItems["ARMOR"] = equipped["ARMOR"].get<std::string>();
        }
    }
}