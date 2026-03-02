#pragma once
#include <stdint.h>
#include "pox_vm8.h"
#include "pox8_platform.h"

void pox8_vm_setup(PoxVm8* vm);
void pox8_vm_load_rom(const uint8_t* rom, uint32_t len, uint16_t load_addr);
void pox8_vm_attach_platform(Pox8Platform* plat);
