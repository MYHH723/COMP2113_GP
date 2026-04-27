#include "battlesystem.h"
#include "player.h"
#include "monster.h"
#include <iostream>
#include <random>
#include <sstream>
#include <iomanip>

std::mt19937 gen(std::random_device{}());

BattleSystem::BattleSystem() 
    : player(nullptr), currentMonster(nullptr), round_count(0), 
      isBattleActive(false), lastResult(BattleResult::ONGOING), reward{0, 0} {
    battleLog.clear();
    battleLog.push_back("Battle system initialized");
}

BattleSystem::~BattleSystem() {
    player = nullptr;
    currentMonster = nullptr;
}

void BattleSystem::initBattle(Player* p, Monster* m) {
    player = p;
    currentMonster = m;
    round_count = 0;
    isBattleActive = true;
    lastResult = BattleResult::ONGOING;
    battleLog.clear();
    std::string log = "Battle started!";
    if (currentMonster) log += " Encounter: " + currentMonster->getName();
    battleLog.push_back(log);
}

void BattleSystem::startBattle() {
    if (!player || !currentMonster) {
        battleLog.push_back("Error: Player or monster missing!");
        return;
    }
    
    isBattleActive = true;
    battleLog.push_back("=== Battle Start ===");
}

void BattleSystem::endBattle() {
    isBattleActive = false;
    
    std::string resultStr;
    switch(lastResult) {
        case BattleResult::PLAYER_WIN: resultStr = "Player Wins"; break;
        case BattleResult::PLAYER_LOSE: resultStr = "Player Loses"; break;
        case BattleResult::PLAYER_FLEE: resultStr = "Player Fled"; break;
        case BattleResult::ONGOING: resultStr = "Battle Interrupted"; break;
    }
    
    battleLog.push_back("Battle ended: " + resultStr);
    battleLog.push_back("=== Battle End ===");
}

BattleResult BattleSystem::executeBattleRound() {
    if (!isBattleActive) {
        battleLog.push_back("Error: No active battle");
        return lastResult;
    }
    
    if (!player || !currentMonster) {
        battleLog.push_back("Error: Player or monster missing");
        isBattleActive = false;
        return lastResult;
    }
    
    if (!currentMonster->get_isAlive()) {
        battleLog.push_back("Monster defeated! Player wins!");
        isBattleActive = false;
        lastResult = BattleResult::PLAYER_WIN;
        return lastResult;
    }
    
    if (!player->get_isAlive()) {
        battleLog.push_back("Player defeated!");
        isBattleActive = false;
        lastResult = BattleResult::PLAYER_LOSE;
        return lastResult;
    }
    
    round_count++;
    battleLog.push_back("--- Round " + std::to_string(round_count) + " ---");
    std::uniform_int_distribution<> int_dist(1, player->get_HP());     
    float dice = int_dist(gen)/DEFAULT_HP;  // Random float between 0 and 1 based on player's HP
    if(dice < 0.1f) {
        playerFlee();
    } else if(dice < 0.2f) {
        playerAttack();
        if (currentMonster->get_isAlive()) {
            monsterAttack();
        }  
    } else if(dice < 0.4f){
        playerDefend();
        monsterAttack();  
    } else{
        playerAttack();
        if (currentMonster->get_isAlive()) {
            monsterAttack();
        }
    }
    return lastResult;
}

int BattleSystem::playerAttack() {
    if (!isBattleActive) {
        battleLog.push_back("Error: Battle not started");
        return 0;
    }
    
    if (!player || !currentMonster) {
        battleLog.push_back("Error: Player or monster missing");
        return 0;
    }
    
    float playerAtk = player->get_ATK();
    std::uniform_real_distribution<float> atkDist(0.8f, 1.2f);
    float damageMultiplier = atkDist(gen);
    int baseDamage = static_cast<int>(playerAtk * damageMultiplier);
    
    int monsterDef = currentMonster->getDEF();
    int actualDamage = baseDamage - monsterDef;
    if (actualDamage < 1) actualDamage = 1;
    
    battleLog.push_back("Player attacks " + currentMonster->getName() + 
                       ", dealing " + std::to_string(actualDamage) + " damage");
    
    currentMonster->takeDamage(actualDamage);
    
    if (!currentMonster->get_isAlive()) {
        battleLog.push_back("Defeated " + currentMonster->getName() + "!");
        isBattleActive = false;
        lastResult = BattleResult::PLAYER_WIN;
        float reward_exp = static_cast<float>(currentMonster->getExpReward());
        float reward_gold = static_cast<float>(currentMonster->getGoldReward());
        reward[0] += reward_exp;
        reward[1] += reward_gold;
        battleLog.push_back("Gained " + std::to_string(reward_exp) + " EXP and " + 
                std::to_string(reward_gold) + " Gold");
    }
    battleLog.push_back("Monster remaining HP: " + std::to_string(currentMonster->getHP()));
    return actualDamage;
}

int BattleSystem::monsterAttack() {
    if (!isBattleActive) {
        battleLog.push_back("Error: Battle not started");
        return 0;
    }
    
    if (!player || !currentMonster) {
        battleLog.push_back("Error: Player or monster missing");
        return 0;
    }
    
    int damage = currentMonster->attackPlayer(*player);
    battleLog.push_back("Player remaining HP: " + std::to_string(player->get_HP()) + "/" + std::to_string(player->get_maxHP()));
    if (damage > 0) {
        battleLog.push_back(currentMonster->getName() + " attacks player, dealing " + 
                           std::to_string(damage) + " damage");
        
        if (!player->get_isAlive()) {
            battleLog.push_back("Player defeated!");
            isBattleActive = false;
            lastResult = BattleResult::PLAYER_LOSE;
        }
    }
    
    return damage;
}

bool BattleSystem::playerFlee() {
    if (!isBattleActive) {
        battleLog.push_back("Error: Battle not started");
        return false;
    }
    
    std::uniform_int_distribution<int> fleeChance(1, 100);
    int chance = fleeChance(gen);
    
    if (chance <= 70) {
        battleLog.push_back("Player successfully fled!");
        isBattleActive = false;
        lastResult = BattleResult::PLAYER_FLEE;
        return true;
    } else {
        battleLog.push_back("Failed to flee!");
        return false;
    }
}

bool BattleSystem::playerDefend() {
    if (!isBattleActive) {
        battleLog.push_back("Error: Battle not started");
        return false;
    }
    
    battleLog.push_back("Player chooses to defend");
    return true;
}

BattleResult BattleSystem::getLastResult() const { return lastResult; }
int BattleSystem::getRoundCount() const { return round_count; }
bool BattleSystem::get_isBattleActive() const { return isBattleActive; }
std::vector<std::string> BattleSystem::getBattleLog() const { return battleLog; }

std::string BattleSystem::showBattleLog() {
    std::stringstream ss;
    ss << "=== Battle Log ===" << std::endl;
    
    if (battleLog.empty()) {
        ss << "(No battle log)" << std::endl;
    } else {
        for (const auto& log : battleLog) {
            ss << log << std::endl;
        }
    }
    
    return ss.str();
}

void BattleSystem::applyRewards() {
    player->change_EXP(reward[0]);
    player->change_Money(reward[1]);
}