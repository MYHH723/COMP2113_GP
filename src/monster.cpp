#include "monster.h"
#include "player.h"
#include <iostream>
#include <random>
#include <string>
#include <cmath>

std::mt19937 gen(std::random_device{}());

Monster::Monster() 
    : name(""), hp(0), maxHp(0), atk(0), def(0), 
      exp_reward(0), gold_reward(0), level(1), isAlive(true) {
}

Monster::~Monster() {
}

void Monster::initMonster(int difficulty, int monsterLevel) {
    level = monsterLevel;
    
    float difficultyMultiplier = 1.0f;
    switch(difficulty) {
        case 1: difficultyMultiplier = 0.8f; break;
        case 2: difficultyMultiplier = 1.0f; break;
        case 3: difficultyMultiplier = 1.5f; break;
    }
    
    if (level <= 2) {
        name = "Goblin";
        maxHp = static_cast<int>(30 * difficultyMultiplier * (1 + 0.1 * level));
        atk = static_cast<int>(8 * difficultyMultiplier * (1 + 0.1 * level));
        def = static_cast<int>(2 * difficultyMultiplier);
        exp_reward = 10 + level * 5;
        gold_reward = 5 + level * 3;
    } else if (level <= 4) {
        name = "Skeleton Warrior";
        maxHp = static_cast<int>(40 * difficultyMultiplier * (1 + 0.1 * level));
        atk = static_cast<int>(10 * difficultyMultiplier * (1 + 0.1 * level));
        def = static_cast<int>(3 * difficultyMultiplier);
        exp_reward = 15 + level * 5;
        gold_reward = 8 + level * 3;
    } else if (level <= 6) {
        name = "Dark Mage";
        maxHp = static_cast<int>(35 * difficultyMultiplier * (1 + 0.1 * level));
        atk = static_cast<int>(15 * difficultyMultiplier * (1 + 0.1 * level));
        def = static_cast<int>(1 * difficultyMultiplier);
        exp_reward = 20 + level * 5;
        gold_reward = 10 + level * 3;
    } else {
        name = "Dungeon Lord";
        maxHp = static_cast<int>(100 * difficultyMultiplier * (1 + 0.1 * level));
        atk = static_cast<int>(20 * difficultyMultiplier * (1 + 0.1 * level));
        def = static_cast<int>(5 * difficultyMultiplier);
        exp_reward = 50 + level * 10;
        gold_reward = 30 + level * 5;
    }
    
    hp = maxHp;
    isAlive = true;
}

std::string Monster::getName() const { return name; }
int Monster::getHP() const { return hp; }
int Monster::getMaxHP() const { return maxHp; }
int Monster::getATK() const { return atk; }
int Monster::getDEF() const { return def; }
int Monster::getExpReward() const { return exp_reward; }
int Monster::getGoldReward() const { return gold_reward; }
int Monster::getLevel() const { return level; }
bool Monster::get_isAlive() const { return isAlive; }

void Monster::setHP(int new_hp) {
    hp = new_hp;
    if (hp < 0) hp = 0;
    if (hp > maxHp) hp = maxHp;
    isAlive = (hp > 0);
}

void Monster::setATK(int new_atk) {
    atk = new_atk;
    if (atk < 0) atk = 0;
}

void Monster::setDEF(int new_def) {
    def = new_def;
    if (def < 0) def = 0;
}

void Monster::set_isAlive(bool alive) { isAlive = alive; }

void Monster::takeDamage(int damage) {
    if (!isAlive) return;
    
    int actualDamage = damage - def;
    if (actualDamage < 1) actualDamage = 1;
    
    hp -= actualDamage;
    if (hp < 0) hp = 0;
    
    if (hp == 0) isAlive = false;
}

int Monster::attackPlayer(Player& player) {
    if (!isAlive) return 0;
    
    std::uniform_int_distribution<int> damageDist(
        static_cast<int>(atk * 0.8), 
        static_cast<int>(atk * 1.2)
    );
    int baseDamage = damageDist(gen);
    
    float playerDef = player.get_DEF();
    int actualDamage = baseDamage - static_cast<int>(playerDef);
    if (actualDamage < 1) actualDamage = 1;
    
    player.change_HP(-static_cast<float>(actualDamage));
    
    return actualDamage;
}

void Monster::die() {
    isAlive = false;
    hp = 0;
}

std::string Monster::getMonsterInfo() const {
    std::string info = "Monster: " + name + "\n";
    info += "Level: " + std::to_string(level) + "\n";
    info += "HP: " + std::to_string(hp) + "/" + std::to_string(maxHp) + "\n";
    info += "ATK: " + std::to_string(atk) + "\n";
    info += "DEF: " + std::to_string(def) + "\n";
    info += "Status: " + std::string(isAlive ? "Alive" : "Dead") + "\n";
    info += "EXP Reward: " + std::to_string(exp_reward) + "\n";
    info += "Gold Reward: " + std::to_string(gold_reward) + "\n";
    
    return info;
}