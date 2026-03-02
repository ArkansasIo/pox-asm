#include "pox8_bios.h"
#include "pox8_eeprom.h"

void pox8_bios_reset(Pox8Platform* plat) {
  pox8_platform_draw_text(plat, "POX BIOS: init I/O");
  pox8_eeprom_init();
  pox8_platform_draw_text(plat, "POX BIOS: ok");
}
