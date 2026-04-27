#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <map>
#include <list>
#include <vector>
#include "item.h"
#include <nlohmann/json.hpp>

// Player class - manages player character attributes and inventory
class Player : public Panel {
private:
    std::string playerName;
    std::map<std::string, float> state;  // Stores level, ATK, DEF, HP, EXP, Money
    Inventory* inventory;
    std::map<std::string, std::string> equippedItems; 
    float maxHP; 
    bool isAlive;
    bool isPoisoned;
    bool isStunned;

public:
    Player();
    Player(std::string name);
    ~Player();

    // Getters
    std::map<std::string, float> get_state() const;
    int get_Level() const;
    float get_ATK() const;
    float get_DEF() const;
    float get_HP() const;
    float get_EXP() const;
    float get_Money() const;
    float get_maxHP() const;
    Inventory* get_inventory() const { return inventory; }
    bool get_isAlive() const;
    bool get_isPoisoned() const;
    bool get_isStunned() const;

    // Setters / State modifiers
    void change_state(const std::string& key, float value);
    void change_ATK(float amount);
    void change_DEF(float amount);
    void change_HP(float amount);
    void change_EXP(float amount);
    void change_Money(float amount);
    void set_isAlive(bool alive);
    void set_isPoisoned(bool poisoned);
    void set_isStunned(bool stunned);

    // Inventory operations
    void add_item(const std::string& itemName);
    void remove_item(const std::string& itemName);
    void use_item(const std::string& itemName);
    void show_items() const;
    void sort_items();
    std::string itemToString(const Item& item);

    // Panel overrides
    void show_status() override;     
    void show_inventory() override;  

    // Other methods
    void equip_weapon(const std::string& itemName);
    void equip_armor(const std::string& itemName);
    void level_up();
    float Player::parseEffectValue(const std::string& itemStr) const;
    nlohmann::json toJson() const;
    void fromJson(const nlohmann::json& j);

};

// Panel class - displays player attributes and inventory
class Panel {
private:
    std::string title;
    std::string statusText;
    std::string conditionText;
    std::string inventoryPreview;

public:
    Panel();
    ~Panel();

    virtual void show_status();
    virtual void show_inventory();

};

// Inventory class - manages items
class Inventory {
private:
    std::list<std::string> items;
    int capacity;

public:
    Inventory();
    ~Inventory();

    const std::list<std::string>& get_items() const { return items; }
    void clear_items() { items.clear(); }
    bool add_item(const std::string& itemName);
    bool remove_item(const std::string& itemName);
    bool use_item(const std::string& itemName);
    void show_items() const;
    void sort_items();
    std::string get_item(const std::string& itemName);
    int get_capacity() const;
    int get_current_size() const;
};

// Global constants
const float DEFAULT_ATK = 100.0f;
const float DEFAULT_DEF = 100.0f;
const float DEFAULT_HP = 1000.0f;
const float DEFAULT_EXP = 0.0f;
const float DEFAULT_MONEY = 100.0f;
const int MAX_INVENTORY_SIZE = 20;
const std::string SAVE_FILE = "save.json";

#endif // PLAYER_H
