#ifndef TRAP_H
#define TRAP_H

#include <string>

// Forward declaration
class Player;

// Trap type enumeration
enum class TrapType {
    SPIKE_PIT,
    POISON_GAS,
    FALLING_BLOCK,
    FIRE_BLAST
};

// Trap class - represents a room trap
class Trap {
private:
    TrapType trapType;
    int damage_min;
    int damage_max;
    int trigger_count;  // How many times triggered
    bool isActive;
    std::string description;

public:
    Trap();
    ~Trap();

    // Initialization
    void initTrap(TrapType type, int difficulty);

    // Getters
    TrapType getTrapType() const;
    int getDamageMin() const;
    int getDamageMax() const;
    int getTriggerCount() const;
    bool get_isActive() const;
    std::string getDescription() const;

    // Setters
    void setActive(bool active);
    void setTriggerCount(int count);

    // Trap actions
    int activateTrap();  // Returns damage dealt
    void triggerTrap(Player& player);

    // Display
    std::string getTrapInfo() const;
};

#endif // TRAP_H
