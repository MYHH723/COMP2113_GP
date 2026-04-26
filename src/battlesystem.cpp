#include "battlesystem.h"
#include "player.h"
#include "monster.h"
#include <iostream>
#include <random>
#include <sstream>
#include <iomanip>

std::mt19937 BattleSystem::gen(std::random_device{}());

BattleSystem::BattleSystem() 
    : player(nullptr), currentMonster(nullptr), round_count(0), 
      isBattleActive(false), lastResult(BattleResult::ONGOING) {
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
        return BattleResult::PLAYER_LOSE;
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
        
        player->change_EXP(static_cast<float>(currentMonster->getExpReward()));
        player->change_Money(static_cast<float>(currentMonster->getGoldReward()));
    }
    
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

std::string BattleSystem::showBattleStatus() {
    std::stringstream ss;
    ss << "=== Battle Status ===" << std::endl;
    
    if (!isBattleActive) {
        ss << "No active battle" << std::endl;
        return ss.str();
    }
    
    ss << "Round: " << round_count << std::endl;
    
    if (player) {
        ss << "Player HP: " << std::fixed << std::setprecision(1) << player->get_HP() 
           << " | ATK: " << player->get_ATK() 
           << " | DEF: " << player->get_DEF() << std::endl;
    }
    
    if (currentMonster) {
        ss << "Monster: " << currentMonster->getName() << std::endl;
        ss << "Monster HP: " << currentMonster->getHP() 
           << "/" << currentMonster->getMaxHP() 
           << " | ATK: " << currentMonster->getATK() 
           << " | DEF: " << currentMonster->getDEF() << std::endl;
    }
    
    ss << "Battle Status: " << (isBattleActive ? "Active" : "Ended") << std::endl;
    
    return ss.str();
}

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