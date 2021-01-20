#include <stdio.h>
#include <stdlib.h>
#include <alloca.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>

#include <inttypes.h>
#include <libgen.h>
#include <unistd.h>

#include "vector.h"
#include "error_helpers.h"
#include "dirstack.h"
#include "config_parser.h"
#include "utils.h"

void movecwd(const struct ent * e) {
  char * base = basename(e->name);
  char * dst = pathcat(".", base);
  
  fmove(e->name, dst);

  free(dst);
}

int main(int argc, char *argv[]) {
  struct config_options co;
  parse_config(&co);

  struct vec * stack = getdirs(co.escrow);
  struct ent ** dirs = stack->buf;

  if(stack->size > 0)
    movecwd(dirs[0]);

  for (int i = 0; i < stack->size; ++i) {
    printf("%d %s\n", i, dirs[i]->name);
  }

  return 0;
}
