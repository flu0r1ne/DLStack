#include "arg_parser.h"
#include "config_parser.h"
#include "subcommands.h"
#include "dl_stack.h"

#include "lib/extlibs.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
  struct dls_args * args = parse_dls_args(argc, argv);

  if(args->help) {
    print_help(args);
    return 0;
  } else if(args->version) {
    print_version();
    return 0;
  }
  
  struct config_options config;
  parse_config(&config);

  struct dl_stack * stack = read_entries(config.downloads_path);
  quicksort(stack->entries, 0, stack->size - 1);

  switch(args->cmd) {
    case SFM_POP:
      dls_pop(args, stack);
      break;
    case SFM_LIST:
      dls_list(args, stack);
      break;
    case SFM_TOP:
      dls_top(args, stack);
      break;
    case SFM_DISCARD:
      dls_discard(args, stack);
      break;
    case SFM_PUSH:
      dls_push(args, &config);
      break;
    default:
      dls_list(args, stack);
      break;
  }

  free_dl_stack(stack);
  free(args);
  free(config.downloads_path);

  return 0;
}
