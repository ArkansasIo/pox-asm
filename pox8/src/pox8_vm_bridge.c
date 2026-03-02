#include "pox8_vm_bridge.h"
#include "pox8_records.h"
#include "pox8_eeprom.h"
#include <string.h>

static uint8_t g_mem[POX_VM8_MEM_SIZE];
static Pox8Platform* g_plat = 0;

static uint8_t bus_rd(void* user, uint16_t addr) {
  (void)user;
  return g_mem[addr];
}

static void bus_wr(void* user, uint16_t addr, uint8_t v) {
  (void)user;
  g_mem[addr] = v;
}

static uint32_t bus_sys(void* user, uint8_t call_id, uint32_t a, uint32_t b) {
  (void)user;
  switch (call_id) {
    case 0x01: {
      const char* s = (const char*)&g_mem[(uint16_t)a];
      if (g_plat) pox8_platform_draw_text(g_plat, s);
      return 0;
    }
    case 0x03: {
      uint16_t addr = (uint16_t)a;
      uint8_t tone = g_mem[addr];
      if (g_plat) pox8_platform_beep(g_plat, tone);
      return 0;
    }
    case 0x10: {
      Pox8Creature c;
      if (pox8_load_creature((uint16_t)a, &c) != 0) return 1;
      uint16_t out = (uint16_t)b;
      g_mem[out+0] = (uint8_t)(c.id & 0xFF);
      g_mem[out+1] = (uint8_t)(c.id >> 8);
      g_mem[out+2] = c.type;
      g_mem[out+3] = c.rank;
      g_mem[out+4] = (uint8_t)(c.hp & 0xFF);
      g_mem[out+5] = (uint8_t)(c.hp >> 8);
      g_mem[out+6] = c.atk;
      g_mem[out+7] = c.def;
      g_mem[out+8] = c.spd;
      g_mem[out+9] = c.spriteIndex;
      return 0;
    }
    default: return 0xFFFFFFFFu;
  }
}

void pox8_vm_setup(PoxVm8* vm) {
  memset(g_mem, 0, sizeof(g_mem));
  PoxVm8Bus bus;
  bus.user = 0;
  bus.rd = bus_rd;
  bus.wr = bus_wr;
  bus.sys = bus_sys;
  pox_vm8_init(vm, bus);
  pox_vm8_reset(vm, 0x8000);
}

void pox8_vm_load_rom(const uint8_t* rom, uint32_t len, uint16_t load_addr) {
  if ((uint32_t)load_addr + len > POX_VM8_MEM_SIZE) return;
  memcpy(&g_mem[load_addr], rom, len);
}

void pox8_vm_attach_platform(Pox8Platform* plat) { 
  g_plat = plat; 
}
