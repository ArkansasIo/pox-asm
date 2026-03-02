#include "pox8_platform.h"
#include <stdio.h>

void pox8_platform_init(Pox8Platform* p) {
  p->buttons = 0;
  p->ticks = 0;
}

void pox8_platform_poll(Pox8Platform* p) {
  p->buttons = 0;
  p->ticks++;
}

void pox8_platform_draw_text(Pox8Platform* p, const char* msg) {
  (void)p;
  printf("[POX8 LCD] %s\n", msg);
}

void pox8_platform_beep(Pox8Platform* p, uint8_t tone) {
  (void)p;
  printf("[POX8 BEEP] tone=%u\n", (unsigned)tone);
}

void pox8_platform_delay_ms(uint16_t ms) {
  (void)ms;
}
