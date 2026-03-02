#pragma once
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define POX_VM8_MEM_SIZE 65536

typedef struct {
  uint8_t  A, X, Y;
  uint8_t  F;
  uint16_t PC;
  uint16_t SP;
} PoxVm8Regs;

typedef struct PoxVm8 PoxVm8;

typedef uint8_t  (*PoxVm8ReadFn)(void* user, uint16_t addr);
typedef void     (*PoxVm8WriteFn)(void* user, uint16_t addr, uint8_t v);
typedef uint32_t (*PoxVm8SysFn)(void* user, uint8_t call_id, uint32_t a, uint32_t b);

typedef struct {
  void* user;
  PoxVm8ReadFn  rd;
  PoxVm8WriteFn wr;
  PoxVm8SysFn   sys;
} PoxVm8Bus;

struct PoxVm8 {
  PoxVm8Regs r;
  PoxVm8Bus  bus;
  uint8_t    halted;
};

enum {
  OP_NOP  = 0x00,
  OP_LDAI = 0x10,
  OP_LDXI = 0x11,
  OP_LDYI = 0x12,
  OP_LDAM = 0x13,
  OP_STAM = 0x14,
  OP_ADDI = 0x20,
  OP_SUBI = 0x21,
  OP_CMPI = 0x22,
  OP_JMP  = 0x30,
  OP_JZ   = 0x31,
  OP_JNZ  = 0x32,
  OP_CALL = 0x33,
  OP_RET  = 0x34,
  OP_SYS  = 0x40,
  OP_HLT  = 0xFF
};

void pox_vm8_init(PoxVm8* vm, PoxVm8Bus bus);
void pox_vm8_reset(PoxVm8* vm, uint16_t pc);
int  pox_vm8_step(PoxVm8* vm);
int  pox_vm8_run(PoxVm8* vm, uint32_t max_steps);

#ifdef __cplusplus
}
#endif
