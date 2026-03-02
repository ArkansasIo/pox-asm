#include "pox64_game.h"
#include "pox64_records.h"
#include "pox64_log.h"
#include "pox64_save.h"

namespace pox64 {

bool Game::init(const std::string& packPath) {
  if (!m_pack.load(packPath)) return false;
  
  std::vector<uint8_t> creaturesBlob;
  if (!m_pack.get_by_name("creatures.bin", creaturesBlob)) {
    log_err("Game: creatures.bin missing from pack");
    return false;
  }
  
  std::string err;
  if (!load_creatures_from_packed20(creaturesBlob, m_world.creatures, err)) {
    log_err("Game: load creatures failed: " + err);
    return false;
  }
  
  World saved;
  if (load_world("pox_save.bin", saved)) {
    m_world.tick = saved.tick;
    m_world.selected = (saved.selected < m_world.creatures.size()) ? saved.selected : 0;
    log_info("Game: loaded save pox_save.bin");
  } else {
    log_info("Game: no save; starting fresh");
    m_world.tick = 0;
    m_world.selected = 0;
  }
  
  return true;
}

void Game::tick() {
  m_world.tick++;
  
  if ((m_world.tick % 60ull) == 0 && !m_world.creatures.empty()) {
    m_world.selected = (m_world.selected + 1) % (uint32_t)m_world.creatures.size();
    const auto& c = m_world.creatures[m_world.selected];
    
    log_info("Tick " + std::to_string(m_world.tick) +
      " Selected=" + std::to_string(m_world.selected) +
      " id=" + std::to_string(c.id) +
      " hp=" + std::to_string(c.hp) +
      " atk=" + std::to_string(c.atk) +
      " def=" + std::to_string(c.def) +
      " spd=" + std::to_string(c.spd));
    
    (void)save_world("pox_save.bin", m_world);
  }
}

}
