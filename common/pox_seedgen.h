#pragma once
#include <stdint.h>
#include "pox_rng.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct { uint64_t v; } PoxSeed;

typedef struct {
  uint32_t galaxy_id;
  uint32_t system_id;
  uint32_t star_class;
  uint32_t planet_count;
} PoxSystemGen;

typedef struct {
  uint32_t planet_id;
  uint32_t biome;
  uint32_t size_class;
  double   gravity;
} PoxPlanetGen;

PoxSeed pox_seed_make(uint64_t root, uint32_t galaxy_id, uint32_t system_id);
void    pox_gen_system(PoxSeed s, PoxSystemGen* out);
void    pox_gen_planet(PoxSeed s, uint32_t planet_index, PoxPlanetGen* out);

#ifdef __cplusplus
}
#endif
