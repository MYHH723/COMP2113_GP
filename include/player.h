#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <map>
#include <list>
#include <vector>
#include "item.h"

// Player class - manages player character attributes and inventory
class Player {
private:
    std::map<std::string, float> state;  // Stores ATK, DEF, HP, EXP, Money
    Inventory* inventory;
    bool isAlive;
    bool isPoisoned;
    bool isStunned;

public:
    Player();
    ~Player();

    // Getters
    std::map<std::string, float> get_state() const;
    float get_ATK() const;
    float get_DEF() const;
    float get_HP() const;
    float get_EXP() const;
    float get_Money() const;
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

    // Save/Load
    void save();
    void load_save();
    friend void saveGame(const Player& );
    friend void loadGame(Player& );
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

    void show_status(const Player& player);
    void show_inventory(const Player& player);
    void show_condition(const Player& player);
    void show_message(const std::string& message);
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
