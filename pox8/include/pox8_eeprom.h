#pragma once
#include <stdint.h>
#include <stddef.h>

#define POX8_EEPROM_SIZE 256

void pox8_eeprom_init(void);
void pox8_eeprom_read(uint16_t addr, uint8_t* out, size_t len);
void pox8_eeprom_write(uint16_t addr, const uint8_t* in, size_t len);
