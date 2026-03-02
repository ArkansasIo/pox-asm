# POX Game - Complete Implementation Guide

## Current Status
✅ Engine core complete (CPU, sprites, memory, saves)  
⏳ Game logic implementation (this document)

## Game Structure

### Main Menu
```
┌─────────────────────┐
│      P·O·X          │
├─────────────────────┤
│ > BATTLE            │
│   COLLECTION        │
│   INFECT            │
│   TRADE             │
│   OPTIONS           │
└─────────────────────┘
```

### 1. BATTLE Mode

**Flow:**
1. Select your creature from collection
2. Random enemy appears
3. Turn-based combat
4. Win/Lose/Run outcome

**Battle Screen:**
```
Enemy: ALIEN [HP: ████████░░]
  ATK:9 DEF:6 SPD:2

Your: GRUNT [HP: ██████████]
  ATK:5 DEF:3 SPD:4

> ATTACK  DEFEND
  SPECIAL  RUN
```

**Combat System:**
- **ATTACK:** Deal damage = ATK - enemy DEF
- **DEFEND:** Reduce next damage by 50%
- **SPECIAL:** Use creature ability (costs turn)
- **RUN:** 50% chance to escape

**Turn Order:** Determined by SPD stat

### 2. COLLECTION Mode

**Display:**
```
┌─────────────────────┐
│   COLLECTION 5/32   │
├─────────────────────┤
│ > #001 GRUNT   Lv3  │
│   #002 SCOUT   Lv2  │
│   #003 TANK    Lv1  │
│   #007 BUG     Lv4  │
│   #012 SPORE   Lv1  │
└─────────────────────┘
```

**Details View:**
```
#001 GRUNT
Type: ALIEN
Level: 3
HP: 30  ATK: 5
DEF: 3  SPD: 4

Ability: RUSH
  Quick attack

Battles: 12 wins
```

### 3. INFECT Mode

**Purpose:** Spread POX to unlock creatures

```
┌─────────────────────┐
│   INFECT MODE       │
├─────────────────────┤
│ Infected: 3 devices │
│                     │
│ [IR PORT ACTIVE]    │
│                     │
│ Point at device...  │
│                     │
│ Unlocks: 2 new POX  │
└─────────────────────┘
```

**Mechanics:**
- Simulated IR transmission
- Each infection unlocks random creature
- Track infection count

### 4. TRADE Mode

```
┌─────────────────────┐
│   TRADE MODE        │
├─────────────────────┤
│ Your: GRUNT         │
│                     │
│ <-> CONNECTING...   │
│                     │
│ Their: ???          │
└─────────────────────┘
```

**Features:**
- Link cable simulation
- Trade creatures
- Unlock special variants

### 5. OPTIONS Mode

```
┌─────────────────────┐
│   OPTIONS           │
├─────────────────────┤
│ Sound:    ON        │
│ Contrast: ███░░     │
│ Difficulty: NORMAL  │
│                     │
│ RESET GAME          │
│ ABOUT               │
└─────────────────────┘
```

## Data Structures

### Game State
```c
typedef struct {
  uint8_t state;           // Current screen
  uint8_t menu_cursor;     // Menu selection
  uint16_t active_creature;// Current creature
  uint8_t collection[32];  // Owned creatures
  uint8_t collection_count;
  uint32_t battles_won;
  uint32_t infections;
  uint8_t sound_on;
} PoxGameState;
```

### Battle State
```c
typedef struct {
  uint16_t player_id;
  uint16_t enemy_id;
  uint16_t player_hp;
  uint16_t enemy_hp;
  uint8_t turn;
  uint8_t player_defending;
  uint8_t result; // 0=ongoing, 1=win, 2=lose
} BattleState;
```

## Implementation Files Needed

### Core Game Logic
```
common/pox_game_state.h    - Game state structures
common/pox_battle.h/c      - Battle system
common/pox_menu.h/c        - Menu navigation
common/pox_ui.h/c          - UI rendering
```

### 8-bit Target
```
pox8/src/pox8_game_logic.c - Main game loop
pox8/src/pox8_battle.c     - Battle implementation
pox8/src/pox8_menu.c       - Menu system
pox8/src/pox8_ui.c         - LCD rendering
```

### 64-bit Target
```
pox64/src/pox64_game_logic.cpp
pox64/src/pox64_battle.cpp
pox64/src/pox64_menu.cpp
pox64/src/pox64_ui.cpp
```

## Creature Database

### Starter Creatures
1. **GRUNT** - Basic alien (ATK:5, DEF:3, SPD:4)
2. **SCOUT** - Fast alien (ATK:4, DEF:2, SPD:7)
3. **TANK** - Heavy alien (ATK:7, DEF:8, SPD:2)

### Infection Unlocks
4. **BUG** - Infection type (ATK:6, DEF:4, SPD:5)
5. **SPORE** - Spreader (ATK:3, DEF:3, SPD:6)
6. **VIRUS** - Corruptor (ATK:8, DEF:2, SPD:4)

### Rare/Boss
7. **CORE** - Boss (ATK:12, DEF:10, SPD:3)
8. **HIVE** - Boss (ATK:10, DEF:12, SPD:2)
9. **WARDEN** - Boss (ATK:15, DEF:8, SPD:5)

## Save Data Format

```
Offset | Size | Description
-------|------|-------------
0-3    | 4    | Magic 'PXSV'
4-7    | 4    | Version
8-11   | 4    | Battles won
12-15  | 4    | Infections
16-47  | 32   | Collection (creature IDs)
48-79  | 32   | Levels
80-81  | 2    | Active creature
82-83  | 2    | Checksum
```

## UI Rendering

### LCD Layout (128x64)
```
┌────────────────────────────┐ 0
│ [TITLE BAR]                │ 8
├────────────────────────────┤
│                            │
│   [MAIN CONTENT]           │ 48
│                            │
├────────────────────────────┤
│ [STATUS BAR]               │ 56
└────────────────────────────┘ 64
```

### Sprite Usage
- UI icons: Hearts, stars, arrows
- Creatures: 8x8 animated sprites
- Bosses: 16x16 sprites
- Text: 8x8 font

## Input Handling

### Button Mapping
- **UP/DOWN:** Navigate menus
- **LEFT/RIGHT:** Adjust values
- **A:** Select/Confirm
- **B:** Back/Cancel

### State Transitions
```
BOOT → MAIN_MENU
MAIN_MENU → BATTLE/COLLECTION/INFECT/TRADE/OPTIONS
BATTLE → BATTLE_MENU → BATTLE_FIGHT → BATTLE_RESULT → MAIN_MENU
```

## Next Steps

1. ✅ Create game state structures
2. ✅ Implement menu system
3. ✅ Implement battle logic
4. ✅ Add UI rendering
5. ✅ Integrate with existing engine
6. ✅ Test all game modes
7. ✅ Add sound effects (beeps)
8. ✅ Polish and balance

## Implementation Priority

### Phase 1: Core Gameplay (Essential)
- Main menu navigation
- Battle system
- Collection view
- Save/load

### Phase 2: Extended Features
- Infect mode
- Trade mode
- Options menu

### Phase 3: Polish
- Animations
- Sound effects
- Difficulty balancing
- Easter eggs

---

**Ready to implement?** This design provides the complete POX game experience on top of your existing engine!
