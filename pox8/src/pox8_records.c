#include "pox8_records.h"
#include "pox_endian.h"
#include "pox_crc16.h"
#include "pox_packed_records.h"

static const uint8_t g_creatures[][POX_CREATURE_REC_SIZE] = {
  {0x01,0x00, 0x02, 0x01, 0x1E,0x00, 0x05,0x03,0x04, 0x07, 0x0A,0x00, 0x02,0x00, 0x01,0x00,0x00,0x00, 0x00,0x00},
  {0x02,0x00, 0x03, 0x02, 0x37,0x00, 0x09,0x06,0x02, 0x03, 0x0B,0x00, 0x04,0x00, 0x02,0x00,0x00,0x00, 0x00,0x00},
};

static void fix_crc_if_needed(uint8_t rec[POX_CREATURE_REC_SIZE]) {
  uint16_t want = pox_crc16_ccitt(rec, 18, 0xFFFF);
  uint16_t have = pox_le16(&rec[18]);
  if (have != want) {
    pox_wrle16(&rec[18], want);
  }
}

size_t pox8_creature_count(void) { 
  return sizeof(g_creatures)/sizeof(g_creatures[0]); 
}

int pox8_load_creature(uint16_t index, Pox8Creature* out) {
  if ((size_t)index >= pox8_creature_count()) return -1;
  
  uint8_t rec[POX_CREATURE_REC_SIZE];
  for (int i=0;i<POX_CREATURE_REC_SIZE;i++) rec[i]=g_creatures[index][i];
  
  fix_crc_if_needed(rec);
  
  uint16_t crc = pox_le16(&rec[18]);
  uint16_t calc = pox_crc16_ccitt(rec, 18, 0xFFFF);
  if (crc != calc) return -1;
  
  out->id = pox_le16(&rec[0]);
  out->type = rec[2];
  out->rank = rec[3];
  out->hp = pox_le16(&rec[4]);
  out->atk = rec[6];
  out->def = rec[7];
  out->spd = rec[8];
  out->spriteIndex = rec[9];
  out->abilityId = pox_le16(&rec[10]);
  out->dropId = pox_le16(&rec[12]);
  out->flags = pox_le32(&rec[14]);
  return 0;
}
