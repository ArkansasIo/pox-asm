# POX Dual Target (8-bit + 64-bit Handheld Game System)

Complete implementation of a POX-style handheld game system with:
- 8-bit firmware-style target (MCU simulation)
- 64-bit modern engine (desktop/mobile ready)
- Virtual 8-bit CPU with bytecode execution
- Full sprite system (128×64 LCD, 1-bit monochrome)
- Asset packing system
- Memory management (arena, pool, ring buffer)
- Deterministic procedural generation
- Binary tools (assembler, hex viewer, record dumper)

## Quick Start

```bash
mkdir build && cd build
cmake ..
cmake --build . -j

# Create asset pack
./pox_packer ../tools/sample_data pox_assets.pak

# Run 64-bit demo
./pox64_demo pox_assets.pak

# Run 8-bit simulation
./pox8_sim

# Assemble VM bytecode
./vmasm ../tools/sample_data/vm_demo.asm vm_demo.bin vm_demo.sym
```

## Architecture

### 8-bit Target (pox8/)
- Simulates early-2000s handheld toy hardware
- Fixed memory layout (ROM/RAM/EEPROM)
- Virtual CPU with custom ISA
- 20-byte packed creature records
- Segment or pixel LCD support

### 64-bit Target (pox64/)
- Modern C++17 engine
- Asset pack system (WAD-like)
- Save file management
- Runtime record expansion
- Full sprite rendering

### Common Systems (common/)
- Virtual 8-bit CPU (pox_vm8)
- Memory allocators (arena, pool, ring)
- Deterministic RNG + procedural generation
- CRC16 validation
- Endian-safe binary I/O
- Full sprite pack (60+ sprites, animations)

### Tools (tools/)
- VM assembler (vmasm) - bytecode compiler
- Asset packer (pox_packer) - creates .pak files
- Hex viewer (hexview) - binary inspection
- Record dumper (recdump) - decode creature records
- Binary patcher (patchbin) - modify binaries

## Sprite System

128×64 monochrome LCD with:
- 60+ sprites (8×8 and 16×16)
- UI icons, digits, font subset
- Player animations (idle, walk, attack, hit)
- 3 alien types with animations
- 3 infection types (bugs, spores, virus)
- 8 items, 8 tiles
- 3 bosses (16×16) with animations

## Memory Layout (8-bit)

```
0x0000-0x3FFF : Program ROM (16 KB)
0x4000-0x7FFF : Data tables / assets
0x0080-0x01FF : RAM (384 bytes)
EEPROM        : Save slots (256 bytes)
```

## VM ISA

- LDAI, LDXI, LDYI - load immediate
- LDAM, STAM - memory access
- ADDI, SUBI, CMPI - arithmetic
- JMP, JZ, JNZ, CALL, RET - control flow
- SYS - syscalls (print, beep, load creature)
- HLT - halt

## File Formats

### Asset Pack (.pak)
- Magic: 'PXAK' (0x4B415850)
- Header: version, count, directory offset
- Directory: 32-byte entries (hash, offset, length, type, compression)

### Creature Record (20 bytes)
- u16 id, u8 type, u8 rank
- u16 hp, u8 atk/def/spd
- u8 spriteIndex, u16 abilityId, u16 dropId
- u32 flags, u16 crc16

### Save File (.bin)
- Magic: 'PXSV' (0x56535850)
- u32 version, u64 tick, u32 selected

## License

Public domain / MIT - use freely
