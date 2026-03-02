#pragma once
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

uint16_t pox_crc16_ccitt(const uint8_t* data, size_t len, uint16_t seed);

#ifdef __cplusplus
}
#endif
