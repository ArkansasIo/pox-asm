#include "pox_seedgen.h"

static uint64_t mix_u64(uint64_t a, uint64_t b) {
  uint64_t x = a ^ (b + 0x9E3779B97F4A7C15ULL + (a<<6) + (a>>2));
  x ^= x >> 30; x *= 0xBF58476D1CE4E5B9ULL;
  x ^= x >> 27; x *= 0x94D049BB133111EBULL;
  x ^= x >> 31;
  return x;
}

PoxSeed pox_seed_make(uint64_t root, uint32_t galaxy_id, uint32_t system_id) {
  uint64_t x = root;
  x = mix_u64(x, ((uint64_t)galaxy_id << 32) | system_id);
  PoxSeed s; s.v = x;
  return s;
}

void pox_gen_system(PoxSeed s, PoxSystemGen* out) {
  PoxRng r; pox_rng_seed(&r, s.v);
  
  out->galaxy_id = (uint32_t)(pox_rng_u32(&r) & 0xFFFF);
  out->system_id = (uint32_t)(pox_rng_u32(&r) & 0xFFFF);
  out->star_class = (uint32_t)(pox_rng_u32(&r) % 7);
  out->planet_count = 1u + (uint32_t)(pox_rng_u32(&r) % 12);
}

void pox_gen_planet(PoxSeed s, uint32_t planet_index, PoxPlanetGen* out) {
  uint64_t ps = mix_u64(s.v, (uint64_t)planet_index);
  PoxRng r; pox_rng_seed(&r, ps);
  
  out->planet_id = planet_index;
  out->biome = (uint32_t)(pox_rng_u32(&r) % 10);
  out->size_class = (uint32_t)(pox_rng_u32(&r) % 6);
  out->gravity = 0.1 + pox_rng_f01(&r) * 2.9;
}
