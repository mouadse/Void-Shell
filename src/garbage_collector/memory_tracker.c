// memory_tracker.c
#include "memory_tracker.h"

static t_mem_block *g_mem_blocks = NULL;

void *gc_malloc(size_t size) {
  void *ptr = malloc(size);
  if (!ptr)
    return NULL;
  t_mem_block *new_block = malloc(sizeof(t_mem_block));
  if (!new_block) {
    free(ptr);
    return NULL;
  }
  new_block->ptr = ptr;
  new_block->next = g_mem_blocks;
  g_mem_blocks = new_block;
  return ptr;
}

void gc_free_all(void) {
  t_mem_block *current = g_mem_blocks;
  while (current) {
    t_mem_block *next = current->next;
    free(current->ptr);
    free(current);
    current = next;
  }
  g_mem_blocks = NULL;
}
