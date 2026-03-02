#include "pox64_records.h"
#include "pox_endian.h"
#include "pox_crc16.h"
#include "pox_packed_records.h"

namespace pox64 {

bool load_creatures_from_packed20(const std::vector<uint8_t>& data, std::vector<Creature>& out, std::string& err) {
  out.clear();
  if (data.size() % POX_CREATURE_REC_SIZE != 0) {
    err = "packed creature data size not multiple of 20";
    return false;
  }
  size_t n = data.size() / POX_CREATURE_REC_SIZE;
  out.reserve(n);
  
  for (size_t i = 0; i < n; i++) {
    const uint8_t* rec = data.data() + i * POX_CREATURE_REC_SIZE;
    
    uint16_t crcHave = pox_le16(rec + 18);
    uint16_t crcCalc = pox_crc16_ccitt(rec, 18, 0xFFFF);
    if (crcHave != crcCalc) {
      err = "crc mismatch at record " + std::to_string(i);
      return false;
    }
    
    Creature c;
    c.id = (uint32_t)pox_le16(rec + 0);
    c.type = rec[2];
    c.rank = rec[3];
    c.hp = (uint32_t)pox_le16(rec + 4);
    c.atk = rec[6];
    c.def = rec[7];
    c.spd = rec[8];
    c.spriteIndex = rec[9];
    c.abilityId = (uint32_t)pox_le16(rec + 10);
    c.dropId = (uint32_t)pox_le16(rec + 12);
    c.flags = (uint64_t)pox_le32(rec + 14);
    out.push_back(c);
  }
  return true;
}

}
