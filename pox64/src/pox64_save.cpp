#include "pox64_save.h"
#include "pox64_io.h"
#include "pox64_log.h"
#include "pox_endian.h"
#include <vector>

namespace pox64 {

// Simple binary save: u32 magic 'PXSV', u32 ver, u64 tick, u32 selected
static constexpr uint32_t MAGIC = 0x56535850; // 'PXSV'
static constexpr uint32_t VER = 1;

bool save_world(const std::string& path, const World& w) {
  std::vector<uint8_t> b;
  b.resize(4+4+8+4);
  pox_wrle32(b.data()+0, MAGIC);
  pox_wrle32(b.data()+4, VER);
  pox_wrle64(b.data()+8, w.tick);
  pox_wrle32(b.data()+16, w.selected);
  
  if (!write_file(path, b)) { 
    log_err("save_world: write failed"); 
    return false; 
  }
  return true;
}

bool load_world(const std::string& path, World& w) {
  std::vector<uint8_t> b;
  if (!read_file(path, b)) return false;
  if (b.size() < 20) return false;
  
  uint32_t magic = pox_le32(b.data()+0);
  uint32_t ver   = pox_le32(b.data()+4);
  if (magic != MAGIC || ver != VER) return false;
  
  w.tick = pox_le64(b.data()+8);
  w.selected = pox_le32(b.data()+16);
  return true;
}

}
