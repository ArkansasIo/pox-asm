#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint64_t s;
} PoxRng;

void     pox_rng_seed(PoxRng* r, uint64_t seed);
uint32_t pox_rng_u32(PoxRng* r);
uint64_t pox_rng_u64(PoxRng* r);
double   pox_rng_f01(PoxRng* r);

#ifdef __cplusplus
}
#endif
