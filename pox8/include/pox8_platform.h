#pragma once
#include <stdint.h>
#include <stddef.h>

typedef struct {
  uint8_t buttons;
  uint32_t ticks;
} Pox8Platform;

enum {
  POX8_BTN_A = 1<<0,
  POX8_BTN_B = 1<<1,
  POX8_BTN_UP = 1<<2,
  POX8_BTN_DOWN = 1<<3,
  POX8_BTN_LEFT = 1<<4,
  POX8_BTN_RIGHT = 1<<5,
};

void pox8_platform_init(Pox8Platform* p);
void pox8_platform_poll(Pox8Platform* p);
void pox8_platform_draw_text(Pox8Platform* p, const char* msg);
void pox8_platform_beep(Pox8Platform* p, uint8_t tone);
void pox8_platform_delay_ms(uint16_t ms);
