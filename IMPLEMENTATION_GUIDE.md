# POX System - Complete Implementation Guide

## Current Status

### ✅ Completed Core Systems

**Common Library (common/)**
- ✅ pox_endian.h - Little-endian binary I/O
- ✅ pox_crc16.h/c - CRC16-CCITT validation
- ✅ pox_packed_records.h - 20-byte creature record format
- ✅ pox_alloc.h/c - Memory allocators (arena, pool, ring)
- ✅ pox_rng.h/c - Deterministic RNG (SplitMix64)
- ✅ pox_seedgen.h/c - Procedural generation system
- ✅ pox_vm8.h/c - Virtual 8-bit CPU (16 opcodes, 64KB address space)
- ✅ pox_spritepack_v1.h/c - 60+ sprites with animations

**Build System**
- ✅ Root CMakeLists.txt
- ✅ common/CMakeLists.txt
- ✅ README.md with quick start guide

## Remaining Implementation

### Phase 1: 8-bit Target (pox8/)

**Files to Create:**

```
pox8/CMakeLists.txt
pox8/include/pox8_platform.h
pox8/include/pox8_bios.h
pox8/include/pox8_eeprom.h
pox8/include/pox8_records.h
pox8/include/pox8_game.h
pox8/include/pox8_vm_bridge.h
pox8/src/pox8_platform_sim.c
pox8/src/pox8_bios.c
pox8/src/pox8_eeprom_sim.c
pox8/src/pox8_records.c
pox8/src/pox8_game.c
pox8/src/pox8_vm_bridge.c
pox8/src/pox8_main.c
```

**Key Features:**
- Simulated MCU platform (buttons, LCD, beeper)
- BIOS/reset initialization
- EEPROM save system (256 bytes)
- Creature record loader with CRC validation
- VM integration with syscalls
- Main game loop

### Phase 2: 64-bit Target (pox64/)

**Files to Create:**

```
pox64/CMakeLists.txt
pox64/include/pox64_log.h
pox64/include/pox64_io.h
pox64/include/pox64_assetpack.h
pox64/include/pox64_records.h
pox64/include/pox64_world.h
pox64/include/pox64_save.h
pox64/include/pox64_game.h
pox64/src/pox64_log.cpp
pox64/src/pox64_io.cpp
pox64/src/pox64_assetpack.cpp
pox64/src/pox64_records.cpp
pox64/src/pox64_world.cpp
pox64/src/pox64_save.cpp
pox64/src/pox64_game.cpp
pox64/src/main.cpp
```

**Key Features:**
- Asset pack system (WAD-like .pak files)
- Record unpacking (20-byte → runtime structs)
- World state management
- Binary save files
- Game loop with tick system

### Phase 3: Tools (tools/)

**Files to Create:**

```
tools/CMakeLists.txt
tools/packer.cpp          - Asset pack creator
tools/vmasm.cpp           - VM bytecode assembler
tools/hexview.cpp         - Binary hex viewer
tools/recdump.cpp         - Creature record decoder
tools/patchbin.cpp        - Binary patcher
tools/sample_data/creatures.bin
tools/sample_data/vm_demo.asm
```

**Tool Descriptions:**

1. **packer** - Creates .pak asset archives
   ```bash
   ./pox_packer input_folder output.pak
   ```

2. **vmasm** - Assembles VM bytecode
   ```bash
   ./vmasm program.asm output.bin symbols.sym
   ```

3. **hexview** - Displays binary files in hex
   ```bash
   ./hexview file.bin [start] [length]
   ```

4. **recdump** - Decodes creature records
   ```bash
   ./recdump creatures.bin
   ```

5. **patchbin** - Patches binary files
   ```bash
   ./patchbin input.bin output.bin offset hexbytes...
   ```

### Phase 4: Sample Data

**creatures.bin** - Two demo creatures (40 bytes):
- Creature 1: id=1, type=2, rank=1, hp=30, atk=5, def=3, spd=4
- Creature 2: id=2, type=3, rank=2, hp=55, atk=9, def=6, spd=2

**vm_demo.asm** - Demo VM program:
```asm
.org 0x8000
boot:
  SYS 0x01, msg_boot, 0x0000
  LDAI 0
  STAM idx
loop:
  LDAM idx
  SYS 0x10, 0x0000, 0x0200
  SYS 0x03, 0x0206, 0x0000
  LDAM idx
  ADDI 1
  CMPI 2
  JNZ noreset
  LDAI 0
noreset:
  STAM idx
  JMP loop
idx: .db 0
msg_boot: .db "POX VM BOOT", 0
```

## Build Instructions

### Prerequisites
- CMake 3.16+
- C11 compiler (GCC, Clang, MSVC)
- C++17 compiler

### Build Steps

```bash
# 1. Create build directory
mkdir build && cd build

# 2. Configure
cmake ..

# 3. Build all targets
cmake --build . -j

# 4. Create sample data
echo -ne '\x01\x00\x02\x01\x1E\x00\x05\x03\x04\x07\x0A\x00\x02\x00\x01\x00\x00\x00\x00\x00' > creatures.bin
echo -ne '\x02\x00\x03\x02\x37\x00\x09\x06\x02\x03\x0B\x00\x04\x00\x02\x00\x00\x00\x00\x00' >> creatures.bin

# 5. Create asset pack
./pox_packer ../tools/sample_data pox_assets.pak

# 6. Run demos
./pox8_sim
./pox64_demo pox_assets.pak
```

## VM ISA Reference

### Opcodes

| Opcode | Mnemonic | Operands | Description |
|--------|----------|----------|-------------|
| 0x00 | NOP | - | No operation |
| 0x10 | LDAI | imm8 | A = immediate |
| 0x11 | LDXI | imm8 | X = immediate |
| 0x12 | LDYI | imm8 | Y = immediate |
| 0x13 | LDAM | addr16 | A = mem[addr] |
| 0x14 | STAM | addr16 | mem[addr] = A |
| 0x20 | ADDI | imm8 | A += immediate |
| 0x21 | SUBI | imm8 | A -= immediate |
| 0x22 | CMPI | imm8 | Set Z if A == immediate |
| 0x30 | JMP | addr16 | PC = addr |
| 0x31 | JZ | addr16 | if Z: PC = addr |
| 0x32 | JNZ | addr16 | if !Z: PC = addr |
| 0x33 | CALL | addr16 | push PC; PC = addr |
| 0x34 | RET | - | PC = pop |
| 0x40 | SYS | id,a16,b16 | Syscall |
| 0xFF | HLT | - | Halt execution |

### Syscalls

| ID | Name | Parameters | Description |
|----|------|------------|-------------|
| 0x01 | print | a=str_addr | Print null-terminated string |
| 0x03 | beep | a=tone_addr | Beep with tone from memory |
| 0x10 | load_creature | a=index, b=out_addr | Load creature record |

## Sprite System

### Sprite IDs (60+ total)

**UI Icons (12):** HEART, SKULL, STAR, ARROW, LOCK, KEY, BATTERY, WIFI, SAVE, LOAD, SOUND, MUTE

**Digits (10):** 0-9

**Font (10):** A-J (subset)

**Player (5):** IDLE, WALK_0, WALK_1, ATK, HIT

**Aliens (6):** GRUNT_0/1, SCOUT_0/1, TANK_0/1

**Bugs (6):** BUG_0/1, SPORE_0/1, VIRUS_0/1

**Items (8):** MEDKIT, BOMB, SHIELD, COIN, CRYSTAL, CHIP, FUEL, MAP

**Tiles (8):** FLOOR, WALL, DOOR_CLOSED, DOOR_OPEN, HAZARD, PANEL, LADDER, TELEPORT

**Bosses (6):** CORE_0/1, HIVE_0/1, WARDEN_0/1 (all 16×16)

### Animation System

```c
// Example: Player walk animation
const PoxAnim* anim = &POX_ANIM_PLAYER_WALK;
uint32_t frame = (tick / (60 / anim->fps)) % anim->count;
PoxSpriteId sprite_id = anim->frames[frame];
const PoxSprite1* sprite = &POX_SPRITEPACK_V1[sprite_id];
// Blit sprite to framebuffer
```

## Memory Layout (8-bit)

```
0x0000-0x3FFF : Program ROM (16 KB)
0x4000-0x7FFF : Data tables / assets
0x0080-0x01FF : RAM (384 bytes)
0x8000-0xFFFF : VM program space
EEPROM        : Save slots (256 bytes)
```

## File Formats

### Asset Pack (.pak)

```
Header (24 bytes):
  u32 magic = 'PXAK' (0x4B415850)
  u16 version
  u16 entry_count
  u64 directory_offset
  u64 reserved

Directory Entry (32 bytes each):
  u64 name_hash (FNV-1a)
  u64 offset
  u64 length
  u32 type
  u32 compression

Data:
  Raw asset blobs
```

### Creature Record (20 bytes)

```
Offset | Type | Field
-------|------|-------
0-1    | u16  | id
2      | u8   | type
3      | u8   | rank
4-5    | u16  | hp
6      | u8   | atk
7      | u8   | def
8      | u8   | spd
9      | u8   | spriteIndex
10-11  | u16  | abilityId
12-13  | u16  | dropId
14-17  | u32  | flags
18-19  | u16  | crc16 (over bytes 0-17)
```

### Save File (.bin)

```
Header:
  u32 magic = 'PXSV' (0x56535850)
  u32 version = 1
  u64 tick
  u32 selected_creature_index
```

## Next Steps

1. **Implement pox8/** - 8-bit firmware simulation
2. **Implement pox64/** - Modern engine
3. **Implement tools/** - Binary utilities
4. **Create sample data** - Demo creatures and VM program
5. **Test build** - Verify all targets compile
6. **Run demos** - Test 8-bit and 64-bit executables

## License

Public domain / MIT - use freely for any purpose.
