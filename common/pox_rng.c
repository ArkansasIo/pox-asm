#include "pox_rng.h"

static uint64_t splitmix64(uint64_t* x) {
  uint64_t z = (*x += 0x9E3779B97F4A7C15ULL);
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
  return z ^ (z >> 31);
}

void pox_rng_seed(PoxRng* r, uint64_t seed) { r->s = seed; }
uint64_t pox_rng_u64(PoxRng* r) { return splitmix64(&r->s); }
uint32_t pox_rng_u32(PoxRng* r) { return (uint32_t)(pox_rng_u64(r) & 0xFFFFFFFFu); }

double pox_rng_f01(PoxRng* r) {
  uint64_t x = pox_rng_u64(r) >> 11;
  return (double)x * (1.0 / 9007199254740992.0);
}
