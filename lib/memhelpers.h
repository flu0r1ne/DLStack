#ifndef __MEMHELPERS_H
#define __MEMHELPERS_H

#include <stdlib.h>
#include <stdio.h>
#include "./handle.h"

#define grown_cap(nr) (((nr) + 16) * 3/2)

static inline void * smalloc(size_t size) {
  void * ptr = malloc(size);

  if(!ptr && size != 0)
    die("Ran out of memory");

  return ptr;
}

static inline void * srealloc(void * ptr, size_t size) {
  ptr = realloc(ptr, size);

  if(!ptr && size != 0)
    die("Ran out of memory");

  return ptr;
}

#define REALLOC_ARRAY(ptr, size) \
  ((ptr) = srealloc((ptr), sizeof(*ptr) * size))

#define DEALLOC_PTR_ARRAY(ptr, size)       \
  do {                                     \
    for(size_t i = 0; i < (size); i++)     \
      free(*((ptr) + i));                  \
  } while(0)

#define ALLOC_GROW(item, nr, alloc)    \
  do {                                 \
    if((nr) > alloc)  {                \
      if (grown_cap(alloc) > (nr))     \
        alloc = grown_cap(nr);         \
      else                             \
        alloc = (nr);                  \
      REALLOC_ARRAY(item, alloc);      \
    }                                  \
  } while(0)

#endif
