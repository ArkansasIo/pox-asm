#pragma once
#include <stdint.h>
#include <stddef.h>

typedef struct {
  uint16_t id;
  uint8_t type;
  uint8_t rank;
  uint16_t hp;
  uint8_t atk, def, spd;
  uint8_t spriteIndex;
  uint16_t abilityId;
  uint16_t dropId;
  uint32_t flags;
} Pox8Creature;

size_t pox8_creature_count(void);
int pox8_load_creature(uint16_t index, Pox8Creature* out);
