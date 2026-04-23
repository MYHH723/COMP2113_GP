# COMP2113_GP - Roguelike Dungeon Game

A C++ based roguelike dungeon exploration game with difficulty scaling, combat system, merchant shop, and inventory management.

## Project Overview

This is a text-based roguelike game where players navigate through procedurally generated dungeon rooms, fight monsters, avoid traps, manage inventory, and purchase items from a merchant. The game features three difficulty levels with scaled monster/trap difficulty.

### Features

- **Difficulty System**: Easy, Normal, and Hard modes with scaled enemy stats and trap damage
- **Combat System**: Turn-based battle mechanics with attack, defend, and flee options
- **Room Generation**: Procedurally generated dungeons with shops, treasure rooms, and boss encounters
- **Player Progression**: Experience system, leveling, stat management, and save/load functionality
- **Inventory System**: Item storage with equipment (Sword/Armor) and consumables (Potions)
- **Shop System**: Buy/sell items with difficulty-based pricing and transaction logging
- **Status Panel**: Real-time display of player stats, inventory, and status effects

## Project Structure

```
COMP2113_GP/
├── src/                      # Source code files
│   └── player.cpp            # Player implementation
│
├── include/                  # Header files
│   ├── game.h               # Game core control
│   ├── player.h             # Player, Inventory, Panel classes
│   ├── room.h               # Room management
│   ├── mapgenerator.h       # Dungeon generation
│   ├── monster.h            # Enemy monsters
│   ├── trap.h               # Room traps
│   ├── battlesystem.h       # Combat mechanics
│   ├── item.h               # Item definitions
│   ├── merchant.h           # Merchant/goods
│   ├── shop.h               # Shop transactions
│   ├── gamelogger.h         # Transaction logging
│   ├── gametester.h         # Testing framework
│   └── consoleUI.h.         # Generate Console Window UI
│
├── data/                     # Game data storage
│   └── save.json            # Player save file (auto-generated)
│
├── docs/                     # Documentation
│   └── HLD/                 # High-Level Design documents
│       ├── Member1_HLD.txt  # Game core & difficulty system
│       ├── Member2_HLD.txt  # Player system & inventory
│       ├── Member3_4_HLD.txt # Combat & room systems
│       └── Member5_HLD.txt  # Shop system
│
└── README.md                 # This file
```

## Module Breakdown

### Member 1: Game Core (game.h)

- Main game loop and flow control
- Difficulty selection and initialization
- Menu system and game state management
- Utility functions (random, delays, screen clearing)
- Global difficulty constants and multipliers

### Member 2: Player System (player.h)

- **Player Class**: Manages HP, ATK, DEF, EXP, Money, status effects
- **Inventory Class**: Item storage with capacity limits (max 20 items)
- **Panel Class**: Display formatted player stats, inventory, and conditions
- Save/load functionality to JSON

### Member 3: Combat System (monster.h, trap.h, battlesystem.h)

- **Monster Class**: Enemy entities with scalable difficulty stats
- **Trap Class**: Environmental hazards (Spike Pit, Poison Gas, Falling Block, Fire Blast)
- **BattleSystem Class**: Turn-based combat with attack/defend/flee mechanics
- Damage calculations with variance
- Battle logging and result tracking

### Member 4: Room & Map System (consoleUI.h, room.h, mapgenerator.h)

- **ConsoleUI Class**: UI demonstrating player's status and inventory level
- **Room Class**: Individual dungeon rooms with content and state tracking
- **MapGenerator Class**: Procedural dungeon generation
- Room types: Normal, Boss, Shop, Treasure
- Difficulty-based content scaling

### Member 5: Shop System (item.h, merchant.h, shop.h, gamelogger.h, gametester.h)

- **Item Class**: Three types (Potion, Sword, Armor) with three grades (Low/Mid/High)
- **Merchant Class**: Goods management with unlimited stock
- **Shop Class**: Buy/sell transactions with gold management
- **GameLogger Class**: Transaction logging
- **GameTester Class**: Testing and bug reporting

## Game Flow

1. **Main Menu**: Player selects New Game, Load Game, or Exit
2. **Difficulty Selection**: Choose Easy/Normal/Hard
3. **Game Loop**:
   - Enter next room
   - Encounter monsters and/or traps
   - Battle or shop interactions
   - Room cleared → Progress to next room
4. **Game End**: Victory (all rooms cleared) or Defeat (player HP = 0)

## Difficulty Scaling

| Difficulty | Total Rooms | Monster HP | Trap Damage | Shop Frequency  |
| ---------- | ----------- | ---------- | ----------- | --------------- |
| Easy       | 10          | 0.8x       | Low         | Every 3-4 rooms |
| Normal     | 15          | 1.0x       | Medium      | Every 4-5 rooms |
| Hard       | 20          | 1.3x       | High        | Every 5-6 rooms |

## Player Stats

- **HP**: 1000 (default), affected by damage/healing
- **ATK**: 100 (default), affects damage dealt
- **DEF**: 100 (default), reduces damage taken
- **EXP**: Experience points for leveling
- **Money**: Gold for purchasing items
- **Status Effects**: Poisoned, Stunned (from items/monsters)

## Item System

### Potions (Consumable)

- **Low Grade**: +5 HP, 5 gold
- **Mid Grade**: +10 HP, 10 gold
- **High Grade**: +15 HP, 15 gold

### Swords (Equipment)

- **Low Grade**: +10 ATK, 15 gold
- **Mid Grade**: +20 ATK, 25 gold
- **High Grade**: +30 ATK, 40 gold

### Armor (Equipment)

- **Low Grade**: +5 DEF, 20 gold
- **Mid Grade**: +10 DEF, 35 gold
- **High Grade**: +15 DEF, 50 gold

## Combat Mechanics

- **Turn Order**: Player acts first, then monster responds
- **Damage Formula**: `Damage = Attacker_ATK - Defender_DEF + Random(-10, +10)`
- **Minimum Damage**: 1 (cannot be reduced below 1)
- **Flee Chance**: 40% success rate to escape battle
- **Defend Action**: Reduces incoming damage by 30%
- **Battle End Conditions**: Player Win, Player Lose, Flee Success

## Compilation

```bash
# Compile all sources
g++ -std=c++11 -o game src/*.cpp

# With debug symbols
g++ -std=c++11 -g -o game src/*.cpp

# Linux compilation
g++ -std=c++11 -pthread -o game src/*.cpp
```

## Platform Support

- Linux Terminal (primary target)
- SSH Environment
- Windows (with proper terminal support)

## Data Files

- **save.json**: Player save state (auto-generated in data/ directory)
- **shop_transaction.log**: Transaction history (auto-generated)

## Future Enhancements

- Additional trap types and mechanics
- Boss-specific abilities
- Skill system for players
- Multiplayer/competitive modes
- Advanced loot/rare items
- UI improvements

## Team Members

- Member 1: Game Core & Flow
- Member 2: Player System & Inventory
- Member 3: Combat System
- Member 4: UI & Room & Map Generation
- Member 5: Shop & Item System

## Notes

- Game runs in terminal environment with text-based UI
- All game data is persisted to save.json
- Transaction logging helps with debugging shop interactions
- Difficulty affects both enemy stats and game duration
- No inventory limit for Potions (consumables)
- Equipment (Sword/Armor) stored in inventory with 20-item capacity
