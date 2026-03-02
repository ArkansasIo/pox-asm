#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Game states
typedef enum {
  GAME_STATE_BOOT,
  GAME_STATE_MAIN_MENU,
  GAME_STATE_BATTLE,
  GAME_STATE_COLLECTION,
  GAME_STATE_INFECT,
  GAME_STATE_TRADE,
  GAME_STATE_OPTIONS,
  GAME_STATE_BATTLE_MENU,
  GAME_STATE_BATTLE_FIGHT,
  GAME_STATE_BATTLE_RESULT
} PoxGameState;

// Main menu items
typedef enum {
  MENU_BATTLE = 0,
  MENU_COLLECTION,
  MENU_INFECT,
  MENU_TRADE,
  MENU_OPTIONS,
  MENU_COUNT
} PoxMainMenuItem;

// Battle actions
typedef enum {
  BATTLE_ACTION_ATTACK = 0,
  BATTLE_ACTION_DEFEND,
  BATTLE_ACTION_SPECIAL,
  BATTLE_ACTION_RUN,
  BATTLE_ACTION_COUNT
} PoxBattleAction;

// Battle state
typedef struct {
  uint16_t player_creature_id;
  uint16_t enemy_creature_id;
  uint16_t player_hp;
  uint16_t enemy_hp;
  uint16_t player_max_hp;
  uint16_t enemy_max_hp;
  uint8_t player_defending;
  uint8_t enemy_defending;
  uint8_t turn_count;
  uint8_t battle_result; // 0=ongoing, 1=win, 2=lose, 3=run
} PoxBattleState;

// Player collection
#define MAX_COLLECTION 32
typedef struct {
  uint16_t creature_ids[MAX_COLLECTION];
  uint8_t creature_levels[MAX_COLLECTION];
  uint8_t creature_count;
  uint8_t active_creature; // index in collection
} PoxCollection;

// Full game state
typedef struct {
  PoxGameState state;
  PoxGameState prev_state;
  uint8_t menu_selection;
  PoxBattleState battle;
  PoxCollection collection;
  uint32_t infection_count;
  uint32_t battles_won;
  uint32_t battles_lost;
} PoxGameContext;

#ifdef __cplusplus
}
#endif
