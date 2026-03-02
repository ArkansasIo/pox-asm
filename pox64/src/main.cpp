#include "pox64_game.h"
#include "pox64_log.h"
#include <iostream>

int main(int argc, char** argv) {
  std::string pack = (argc >= 2) ? argv[1] : "pox_assets.pak";
  
  pox64::Game g;
  if (!g.init(pack)) {
    pox64::log_err("Failed to init. Run the packer tool to create pox_assets.pak");
    pox64::log_info("Example: ./pox_packer tools/sample_data pox_assets.pak");
    return 1;
  }
  
  for (int i = 0; i < 300; i++) g.tick();
  
  pox64::log_info("Done.");
  return 0;
}
