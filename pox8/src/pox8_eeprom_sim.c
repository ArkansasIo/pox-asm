#include "pox8_eeprom.h"
#include <string.h>

static uint8_t g_eeprom[POX8_EEPROM_SIZE];

void pox8_eeprom_init(void) {
  if (g_eeprom[0] == 0xFF && g_eeprom[1] == 0xFF) {
    memset(g_eeprom, 0xFF, sizeof(g_eeprom));
    g_eeprom[0] = 'P';
    g_eeprom[1] = 'X';
  }
}

void pox8_eeprom_read(uint16_t addr, uint8_t* out, size_t len) {
  if (addr + len > POX8_EEPROM_SIZE) return;
  memcpy(out, &g_eeprom[addr], len);
}

void pox8_eeprom_write(uint16_t addr, const uint8_t* in, size_t len) {
  if (addr + len > POX8_EEPROM_SIZE) return;
  memcpy(&g_eeprom[addr], in, len);
}
