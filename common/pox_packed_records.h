#pragma once
#include <stdint.h>

#define POX_CREATURE_REC_SIZE 20

// 20-byte packed record layout (little-endian):
// 0..1  u16 id
// 2     u8  type
// 3     u8  rank
// 4..5  u16 hp
// 6     u8  atk
// 7     u8  def
// 8     u8  spd
// 9     u8  spriteIndex
// 10..11 u16 abilityId
// 12..13 u16 dropId
// 14..17 u32 flags
// 18..19 u16 crc16 (over bytes 0..17)
