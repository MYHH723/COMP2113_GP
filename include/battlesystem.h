#ifndef BATTLESYSTEM_H
#define BATTLESYSTEM_H

#include <string>
#include <vector>

// Forward declarations
class Player;
class Monster;

// BattleResult enumeration
enum struct BattleResult {
    PLAYER_WIN,
    PLAYER_LOSE,
    PLAYER_FLEE,
    ONGOING
};

// BattleSystem class - manages combat logic
class BattleSystem {
private:
    Player* player;
    Monster* currentMonster;
    int round_count;
    bool isBattleActive;
    BattleResult lastResult;
    std::vector<std::string> battleLog;
    float reward[2];

public:
    BattleSystem();
    ~BattleSystem();

    // Initialization
    void initBattle(Player* p, Monster* m);

    // Battle control
    void startBattle();
    void endBattle();
    BattleResult executeBattleRound();

    // Combat actions
    int playerAttack();
    int monsterAttack();
    bool playerFlee();
    bool playerDefend();

    // Getters
    BattleResult getLastResult() const;
    int getRoundCount() const;
    bool get_isBattleActive() const;
    std::vector<std::string> getBattleLog() const;

    // Settlement
    std::string showBattleLog();
    void applyRewards();
};

#endif // BATTLESYSTEM_H
