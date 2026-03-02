#pragma once
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ---------- Arena allocator (linear, reset-only) ----------
typedef struct {
  uint8_t* base;
  size_t   cap;
  size_t   off;
} PoxArena;

void  pox_arena_init(PoxArena* a, void* mem, size_t cap);
void* pox_arena_alloc(PoxArena* a, size_t size, size_t align);
void  pox_arena_reset(PoxArena* a);

// ---------- Fixed-size pool allocator ----------
typedef struct {
  uint8_t* base;
  uint32_t elem_size;
  uint32_t elem_count;
  int32_t  free_head;
} PoxPool;

void  pox_pool_init(PoxPool* p, void* mem, uint32_t elem_size, uint32_t elem_count);
void* pox_pool_alloc(PoxPool* p);
void  pox_pool_free(PoxPool* p, void* ptr);

// ---------- Ring buffer (bytes) ----------
typedef struct {
  uint8_t* buf;
  uint32_t cap;
  uint32_t r;
  uint32_t w;
  uint32_t used;
} PoxRing;

void     pox_ring_init(PoxRing* rb, void* mem, uint32_t cap);
uint32_t pox_ring_write(PoxRing* rb, const uint8_t* in, uint32_t n);
uint32_t pox_ring_read (PoxRing* rb, uint8_t* out, uint32_t n);

#ifdef __cplusplus
}
#endif
