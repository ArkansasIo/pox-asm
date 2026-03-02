#pragma once
#include "pox64_world.h"
#include <string>

namespace pox64 {
  bool save_world(const std::string& path, const World& w);
  bool load_world(const std::string& path, World& w);
}
