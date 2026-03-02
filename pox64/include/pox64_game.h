#pragma once
#include "pox64_assetpack.h"
#include "pox64_world.h"
#include <string>

namespace pox64 {

class Game {
public:
  bool init(const std::string& packPath);
  void tick();
  const World& world() const { return m_world; }

private:
  AssetPack m_pack;
  World m_world;
};

}
