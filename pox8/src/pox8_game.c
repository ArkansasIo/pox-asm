#include "pox8_game.h"
#include "pox8_records.h"
#include "pox8_eeprom.h"
#include "pox8_vm_bridge.h"
#include "pox_vm8.h"
#include <stdio.h>

typedef struct {
  uint8_t initialized;
  uint16_t selected_index;
  PoxVm8 vm;
  uint8_t vm_ready;
} Pox8State;

static Pox8State g;

static void load_state(void) {
  uint8_t buf[2];
  pox8_eeprom_read(10, buf, 2);
  uint16_t v = (uint16_t)buf[0] | ((uint16_t)buf[1] << 8);
  if (v >= pox8_creature_count()) v = 0;
  g.selected_index = v;
}

static void save_state(void) {
  uint8_t buf[2] = { (uint8_t)(g.selected_index & 0xFF), (uint8_t)(g.selected_index >> 8) };
  pox8_eeprom_write(10, buf, 2);
}

void pox8_game_init(Pox8Platform* plat) {
  g.initialized = 1;
  load_state();
  pox8_vm_setup(&g.vm);
  pox8_vm_attach_platform(plat);
  g.vm_ready = 1;
}

void pox8_game_tick(Pox8Platform* plat) {
  if (!g.initialized) pox8_game_init(plat);
  
  if (g.vm_ready) {
    (void)pox_vm8_run(&g.vm, 50);
  }
  
  if ((plat->ticks % 60) == 0) {
    g.selected_index = (uint16_t)((g.selected_index + 1) % pox8_creature_count());
    save_state();
    
    Pox8Creature c;
    if (pox8_load_creature(g.selected_index, &c) == 0) {
      char msg[96];
      snprintf(msg, sizeof(msg), "HOST C%u id=%u hp=%u atk=%u def=%u spd=%u",
               (unsigned)g.selected_index,
               (unsigned)c.id, (unsigned)c.hp, (unsigned)c.atk, (unsigned)c.def, (unsigned)c.spd);
      pox8_platform_draw_text(plat, msg);
    }
  }
}
