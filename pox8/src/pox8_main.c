#include "pox8_platform.h"
#include "pox8_bios.h"
#include "pox8_game.h"

int main(void) {
  Pox8Platform plat;
  pox8_platform_init(&plat);
  pox8_bios_reset(&plat);
  pox8_game_init(&plat);
  
  for (int i = 0; i < 300; i++) {
    pox8_platform_poll(&plat);
    pox8_game_tick(&plat);
    pox8_platform_delay_ms(16);
  }
  return 0;
}
