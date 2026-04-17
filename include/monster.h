#ifndef MONSTER_H
#define MONSTER_H

#include <string>

// Forward declaration
class Player;

// Monster class - represents an enemy monster
class Monster {
private:
    std::string name;
    int hp;
    int maxHp;
    int atk;
    int def;
    int exp_reward;
    int gold_reward;
    int level;
    bool isAlive;

public:
    Monster();
    ~Monster();

    // Initialization
    void initMonster(int difficulty, int level);

    // Getters
    std::string getName() const;
    int getHP() const;
    int getMaxHP() const;
    int getATK() const;
    int getDEF() const;
    int getExpReward() const;
    int getGoldReward() const;
    int getLevel() const;
    bool get_isAlive() const;

    // Setters
    void setHP(int hp);
    void setATK(int atk);
    void setDEF(int def);
    void set_isAlive(bool alive);

    // Combat methods
    void takeDamage(int damage);
    int attackPlayer(Player& player);
    void die();

    // Display
    std::string getMonsterInfo() const;
};

#endif // MONSTER_H
