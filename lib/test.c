#include "extlibs.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {
  
  int * item = NULL;
  size_t nr = 0,
         nc = 0;
  
  #define N (1024 * 1024)
  for (int i = 0; i < N; ++i) {
     ALLOC_GROW(item, nr + 1, nc);
     item[nr++] = rand();
  }

  return 0;
}
