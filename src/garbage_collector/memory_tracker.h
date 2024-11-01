#ifndef MEMORY_TRACKER_H
#define MEMORY_TRACKER_H

#include <stdlib.h>

typedef struct s_mem_block {
  void *ptr;
  struct s_mem_block *next;
} t_mem_block;

void *gc_malloc(size_t size);
void gc_free_all(void);

#endif // MEMORY_TRACKER_H
