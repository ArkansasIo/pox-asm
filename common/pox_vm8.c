#include "pox_vm8.h"

static uint8_t rd8(PoxVm8* vm, uint16_t a) { return vm->bus.rd(vm->bus.user, a); }
static void    wr8(PoxVm8* vm, uint16_t a, uint8_t v) { vm->bus.wr(vm->bus.user, a, v); }

static uint16_t rd16(PoxVm8* vm, uint16_t a) {
  uint8_t lo = rd8(vm, a);
  uint8_t hi = rd8(vm, (uint16_t)(a + 1));
  return (uint16_t)(lo | ((uint16_t)hi << 8));
}

static void setZ(PoxVm8* vm, uint8_t v) {
  if (v == 0) vm->r.F |= 1u; else vm->r.F &= (uint8_t)~1u;
}
static void setC(PoxVm8* vm, uint16_t v) {
  if (v > 255) vm->r.F |= 2u; else vm->r.F &= (uint8_t)~2u;
}
static uint8_t getZ(PoxVm8* vm) { return (uint8_t)(vm->r.F & 1u); }

static void push8(PoxVm8* vm, uint8_t v) { wr8(vm, vm->r.SP--, v); }
static uint8_t pop8(PoxVm8* vm) { return rd8(vm, ++vm->r.SP); }

static void push16(PoxVm8* vm, uint16_t v) {
  push8(vm, (uint8_t)(v & 0xFF));
  push8(vm, (uint8_t)(v >> 8));
}
static uint16_t pop16(PoxVm8* vm) {
  uint8_t hi = pop8(vm);
  uint8_t lo = pop8(vm);
  return (uint16_t)(lo | ((uint16_t)hi << 8));
}

void pox_vm8_init(PoxVm8* vm, PoxVm8Bus bus) {
  vm->bus = bus;
  vm->halted = 0;
  vm->r.A = vm->r.X = vm->r.Y = 0;
  vm->r.F = 0;
  vm->r.PC = 0;
  vm->r.SP = 0xFFFE;
}

void pox_vm8_reset(PoxVm8* vm, uint16_t pc) {
  vm->halted = 0;
  vm->r.A = vm->r.X = vm->r.Y = 0;
  vm->r.F = 0;
  vm->r.PC = pc;
  vm->r.SP = 0xFFFE;
}

int pox_vm8_step(PoxVm8* vm) {
  if (vm->halted) return 0;
  
  uint8_t op = rd8(vm, vm->r.PC++);
  switch (op) {
    case OP_NOP: break;
    
    case OP_LDAI: vm->r.A = rd8(vm, vm->r.PC++); setZ(vm, vm->r.A); break;
    case OP_LDXI: vm->r.X = rd8(vm, vm->r.PC++); break;
    case OP_LDYI: vm->r.Y = rd8(vm, vm->r.PC++); break;
    
    case OP_LDAM: {
      uint16_t a = rd16(vm, vm->r.PC); vm->r.PC += 2;
      vm->r.A = rd8(vm, a); setZ(vm, vm->r.A);
    } break;
    
    case OP_STAM: {
      uint16_t a = rd16(vm, vm->r.PC); vm->r.PC += 2;
      wr8(vm, a, vm->r.A);
    } break;
    
    case OP_ADDI: {
      uint8_t imm = rd8(vm, vm->r.PC++);
      uint16_t s = (uint16_t)vm->r.A + (uint16_t)imm;
      setC(vm, s);
      vm->r.A = (uint8_t)s;
      setZ(vm, vm->r.A);
    } break;
    
    case OP_SUBI: {
      uint8_t imm = rd8(vm, vm->r.PC++);
      int16_t d = (int16_t)vm->r.A - (int16_t)imm;
      vm->r.F &= (uint8_t)~2u;
      vm->r.A = (uint8_t)d;
      setZ(vm, vm->r.A);
    } break;
    
    case OP_CMPI: {
      uint8_t imm = rd8(vm, vm->r.PC++);
      setZ(vm, (uint8_t)(vm->r.A ^ imm));
    } break;
    
    case OP_JMP: {
      uint16_t a = rd16(vm, vm->r.PC);
      vm->r.PC = a;
    } break;
    
    case OP_JZ: {
      uint16_t a = rd16(vm, vm->r.PC); vm->r.PC += 2;
      if (getZ(vm)) vm->r.PC = a;
    } break;
    
    case OP_JNZ: {
      uint16_t a = rd16(vm, vm->r.PC); vm->r.PC += 2;
      if (!getZ(vm)) vm->r.PC = a;
    } break;
    
    case OP_CALL: {
      uint16_t a = rd16(vm, vm->r.PC); vm->r.PC += 2;
      push16(vm, vm->r.PC);
      vm->r.PC = a;
    } break;
    
    case OP_RET: {
      vm->r.PC = pop16(vm);
    } break;
    
    case OP_SYS: {
      uint8_t id = rd8(vm, vm->r.PC++);
      uint16_t a = rd16(vm, vm->r.PC); vm->r.PC += 2;
      uint16_t b = rd16(vm, vm->r.PC); vm->r.PC += 2;
      uint32_t ret = vm->bus.sys(vm->bus.user, id, a, b);
      vm->r.A = (uint8_t)(ret & 0xFF);
      vm->r.X = (uint8_t)((ret >> 8) & 0xFF);
      vm->r.Y = (uint8_t)((ret >> 16) & 0xFF);
      setZ(vm, vm->r.A);
    } break;
    
    case OP_HLT:
      vm->halted = 1;
      break;
    
    default:
      return -1;
  }
  
  return 0;
}

int pox_vm8_run(PoxVm8* vm, uint32_t max_steps) {
  for (uint32_t i = 0; i < max_steps; i++) {
    int rc = pox_vm8_step(vm);
    if (rc != 0) return rc;
    if (vm->halted) return 0;
  }
  return 0;
}
