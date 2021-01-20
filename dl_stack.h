#ifndef __DIR_STACK__H
#define __DIR_STACK__H

#include <aio.h>

struct dl_entry {
  char * path;
  char * name;
  time_t access_time;
};

struct dl_stack {
  struct dl_entry ** entries;
  size_t size;
};

struct dl_stack * read_entries(const char * dl_path);

void free_dl_stack(struct dl_stack * stack);

void quicksort(struct dl_entry ** entries, int low, int high);

#endif
