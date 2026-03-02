#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace pox64 {

struct Creature {
  uint32_t id = 0;
  uint32_t type = 0;
  uint32_t rank = 0;
  uint32_t hp = 0;
  uint32_t atk = 0, def = 0, spd = 0;
  uint32_t spriteIndex = 0;
  uint32_t abilityId = 0;
  uint32_t dropId = 0;
  uint64_t flags = 0;
};

bool load_creatures_from_packed20(const std::vector<uint8_t>& data, std::vector<Creature>& out, std::string& err);

}
