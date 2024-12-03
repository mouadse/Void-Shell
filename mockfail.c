#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

static int malloc_fail_count = 0;
static int fail_after = 2;

void *malloc(size_t size) {
  if (malloc_fail_count >= fail_after) {
    fprintf(stderr, "Simulated malloc failure from LD_PRELOAD\n");
    return NULL;
  }
  malloc_fail_count++;
  void *(*original_malloc)(size_t) = dlsym(RTLD_NEXT, "malloc");
  return original_malloc(size);
}
