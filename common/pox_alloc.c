#include "pox_alloc.h"

static size_t align_up(size_t x, size_t a) {
  return (x + (a - 1)) & ~(a - 1);
}

void pox_arena_init(PoxArena* a, void* mem, size_t cap) {
  a->base = (uint8_t*)mem;
  a->cap = cap;
  a->off = 0;
}

void* pox_arena_alloc(PoxArena* a, size_t size, size_t align) {
  if (align == 0) align = 1;
  size_t off2 = align_up(a->off, align);
  if (off2 + size > a->cap) return 0;
  void* p = a->base + off2;
  a->off = off2 + size;
  return p;
}

void pox_arena_reset(PoxArena* a) { a->off = 0; }

void pox_pool_init(PoxPool* p, void* mem, uint32_t elem_size, uint32_t elem_count) {
  if (elem_size < 4) elem_size = 4;
  p->base = (uint8_t*)mem;
  p->elem_size = elem_size;
  p->elem_count = elem_count;
  p->free_head = (elem_count > 0) ? 0 : -1;
  
  for (uint32_t i = 0; i < elem_count; i++) {
    int32_t next = (i + 1 < elem_count) ? (int32_t)(i + 1) : -1;
    *(int32_t*)(p->base + (size_t)i * elem_size) = next;
  }
}

void* pox_pool_alloc(PoxPool* p) {
  if (p->free_head < 0) return 0;
  int32_t idx = p->free_head;
  uint8_t* elem = p->base + (size_t)idx * p->elem_size;
  p->free_head = *(int32_t*)elem;
  return elem;
}

void pox_pool_free(PoxPool* p, void* ptr) {
  if (!ptr) return;
  uint8_t* b = (uint8_t*)ptr;
  if (b < p->base) return;
  size_t off = (size_t)(b - p->base);
  if (off % p->elem_size != 0) return;
  uint32_t idx = (uint32_t)(off / p->elem_size);
  if (idx >= p->elem_count) return;
  
  *(int32_t*)b = p->free_head;
  p->free_head = (int32_t)idx;
}

void pox_ring_init(PoxRing* rb, void* mem, uint32_t cap) {
  rb->buf = (uint8_t*)mem;
  rb->cap = cap;
  rb->r = rb->w = rb->used = 0;
}

uint32_t pox_ring_write(PoxRing* rb, const uint8_t* in, uint32_t n) {
  uint32_t can = rb->cap - rb->used;
  if (n > can) n = can;
  for (uint32_t i = 0; i < n; i++) {
    rb->buf[rb->w] = in[i];
    rb->w = (rb->w + 1) % rb->cap;
  }
  rb->used += n;
  return n;
}

uint32_t pox_ring_read(PoxRing* rb, uint8_t* out, uint32_t n) {
  if (n > rb->used) n = rb->used;
  for (uint32_t i = 0; i < n; i++) {
    out[i] = rb->buf[rb->r];
    rb->r = (rb->r + 1) % rb->cap;
  }
  rb->used -= n;
  return n;
}
