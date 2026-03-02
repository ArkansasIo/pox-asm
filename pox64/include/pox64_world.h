#pragma once
#include "pox64_records.h"
#include <vector>
#include <cstdint>

namespace pox64 {

struct World {
  uint64_t tick = 0;
  std::vector<Creature> creatures;
  uint32_t selected = 0;
};

}
