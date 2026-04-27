#include "trap.h"
#include "player.h"
#include <iostream>
#include <random>
#include <string>

std::mt19937 gen(std::random_device{}());

Trap::Trap() 
    : trapType(TrapType::SPIKE_PIT), damage_min(0), damage_max(0), 
      trigger_count(0), isActive(true), description("") {
}

Trap::~Trap() {
}

void Trap::initTrap(TrapType type, int difficulty) {
    trapType = type;
    trigger_count = 0;
    isActive = true;
    
    int base_damage_min = 0;
    int base_damage_max = 0;
    
    switch(type) {
        case TrapType::SPIKE_PIT:
            base_damage_min = 8;
            base_damage_max = 12;
            description = "Spikes suddenly emerge from the ground!";
            break;
        case TrapType::POISON_GAS:
            base_damage_min = 5;
            base_damage_max = 8;
            description = "Poisonous gas fills the air!";
            break;
        case TrapType::FALLING_BLOCK:
            base_damage_min = 10;
            base_damage_max = 15;
            description = "Rocks fall from the ceiling!";
            break;
        case TrapType::FIRE_BLAST:
            base_damage_min = 7;
            base_damage_max = 10;
            description = "Fire erupts from the wall!";
            break;
    }
    
    float difficultyMultiplier = 1.0f;
    switch(difficulty) {
        case 1: difficultyMultiplier = 0.8f; break;
        case 2: difficultyMultiplier = 1.0f; break;
        case 3: difficultyMultiplier = 1.5f; break;
    }
    
    damage_min = static_cast<int>(base_damage_min * difficultyMultiplier);
    damage_max = static_cast<int>(base_damage_max * difficultyMultiplier);
}

TrapType Trap::getTrapType() const { return trapType; }
int Trap::getDamageMin() const { return damage_min; }
int Trap::getDamageMax() const { return damage_max; }
int Trap::getTriggerCount() const { return trigger_count; }
bool Trap::get_isActive() const { return isActive; }
std::string Trap::getDescription() const { return description; }

void Trap::setActive(bool active) { isActive = active; }
void Trap::setTriggerCount(int count) { trigger_count = count; }

int Trap::activateTrap() {
    if (!isActive) return 0;
    
    trigger_count++;
    std::uniform_int_distribution<int> damageDist(damage_min, damage_max);
    return damageDist(gen);
}

void Trap::triggerTrap(Player& player) {
    if (!isActive) return;
    
    int damage = activateTrap();
    player.change_HP(-static_cast<float>(damage));
    
    std::uniform_int_distribution<int> chanceDist(1, 100);
    if (chanceDist(gen) <= 30) player.set_isPoisoned(true);
}

std::string Trap::getTrapInfo() const {
    std::string typeStr;
    switch(trapType) {
        case TrapType::SPIKE_PIT: typeStr = "Spike Trap"; break;
        case TrapType::POISON_GAS: typeStr = "Poison Gas Trap"; break;
        case TrapType::FALLING_BLOCK: typeStr = "Falling Block Trap"; break;
        case TrapType::FIRE_BLAST: typeStr = "Fire Blast Trap"; break;
    }
    
    std::string info = "Trap Type: " + typeStr + "\n";
    info += "Description: " + description + "\n";
    info += "Damage Range: " + std::to_string(damage_min) + " - " + 
            std::to_string(damage_max) + "\n";
    info += "Trigger Count: " + std::to_string(trigger_count) + "\n";
    info += "Status: " + std::string(isActive ? "Active" : "Inactive") + "\n";
    
    return info;
}