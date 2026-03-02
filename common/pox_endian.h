#pragma once
#include <stdint.h>

static inline uint16_t pox_le16(const uint8_t* p) {
  return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}
static inline uint32_t pox_le32(const uint8_t* p) {
  return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}
static inline uint64_t pox_le64(const uint8_t* p) {
  return (uint64_t)pox_le32(p) | ((uint64_t)pox_le32(p+4) << 32);
}

static inline void pox_wrle16(uint8_t* p, uint16_t v) { p[0]=(uint8_t)(v); p[1]=(uint8_t)(v>>8); }
static inline void pox_wrle32(uint8_t* p, uint32_t v) {
  p[0]=(uint8_t)(v); p[1]=(uint8_t)(v>>8); p[2]=(uint8_t)(v>>16); p[3]=(uint8_t)(v>>24);
}
static inline void pox_wrle64(uint8_t* p, uint64_t v) {
  pox_wrle32(p, (uint32_t)(v));
  pox_wrle32(p+4, (uint32_t)(v>>32));
}
