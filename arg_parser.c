#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <libgen.h>

#include "lib/handle.h"
#include "arg_parser.h"

#define SFM_VERSION "1.0.0"

#define SFM_USAGE        \
  "Usage: dls\n"         \
  "or: dls <cmd> <args>"

#define SFM_HELP                                               \
  "dls (p)op <number> <path>\n"                                \
  "dls (l)ist\n"                                               \
  "dls (t)op <number>\n"                                       \
  "dls (d)iscard <number>\n"                                   \
  "dls (pu)sh <path>\n"                                        \
  "dls_push <path>\n"                                          \
  "dls --help <cmd>\n"                                         \
  "\n"                                                         \
  "Report bugs to ab[at]flu0r1ne[dot]net\n"


#define POP_HELP                                     \
  "dls (p)op [-s] [<number>] [<path>]\n"             \
  "\n"                                               \
  "Removes the most recent file(s) from the file\n"  \
  "stack and moves them to the working directory"

#define LIST_HELP \
  "dls (l)i(s)t\n"  \
  "\n"            \
  "List the contents of the file stack"

#define TOP_HELP                                      \
  "dls (t)op [-s] [<number>]"                         \
  "\n"                                                \
  "Print the most recent file(s) to the standard output"

#define WARNING \
  "WARNING: This operation will permanently remove file(s) from \n"    \
  "the file system. If used inappropriately, this could result in \n"  \
  "data loss."

#define DISCARD_HELP                                                           \
  "dls (d)iscard [<number>]\n"                                                 \
  "\n"                                                                         \
  "Remove the most recent file(s). To remove all files, pass a wildcard (*)\n" \
  "as the number argument."

#define PUSH_HELP              \
  "dls (pu)sh [<path>]\n"      \
  "\n"                         \
  "Push a file onto the stack"

void dls_help(enum dls_cmd c) {
  switch(c) {
    case SFM_POP:
      printf(POP_HELP "\n");
      break;
    case SFM_LIST:
      printf(LIST_HELP "\n");
      break;
    case SFM_TOP:
      printf(TOP_HELP "\n");
      break;
    case SFM_DISCARD:
      printf(DISCARD_HELP "\n");
      printf("\n" WARNING "\n");
      break;
    case SFM_PUSH:
      printf(PUSH_HELP "\n");
      break;
    case SFM_UNKNOWN:
      printf(SFM_USAGE "\n\n" SFM_HELP);
  } 
}

static unsigned long parse_long_arg(const char * str) {
  if(strcmp(str, "*") == 0)
    return MAX_DEPTH;

  char * end = (char *) str;
  unsigned long u = strtoul(str, &end, 10);
  
  if(!end || u == ULONG_MAX)
    die("Please enter a number between 0 and %lu\n", ULONG_MAX - 1);

  return u; 
}

enum dls_cmd parse_command(const char * cmd) {
  if(!cmd)
    return SFM_UNKNOWN;

  if(strcmp(cmd, "p") == 0 || strcmp(cmd, "pop") == 0)
    return SFM_POP;
  else if(strcmp(cmd, "ls") == 0 || strcmp(cmd, "list") == 0)
    return SFM_LIST;
  else if(strcmp(cmd, "t") == 0 || strcmp(cmd, "top") == 0)
    return SFM_TOP;
  else if(strcmp(cmd, "d") == 0 || strcmp(cmd, "discard") == 0)
    return SFM_DISCARD;
  else if(strcmp(cmd, "pu") == 0 || strcmp(cmd, "push") == 0)
    return SFM_PUSH;

  return SFM_UNKNOWN;
}

static void parse_fmt_subcmd(const char * fmt, struct dls_args * args, int argc, char * argv[]) {
  int choice;
  optind = 1;
  while (true) {
    static struct option long_options[] = {
      {"stream", no_argument, 0, 's'},
      {0,0,0,0}
    };
  
    int option_index = 0;
    choice = getopt_long(argc, argv, "s", long_options, &option_index);

    if(choice == -1)
      break;

    switch(choice) {
      case 's':
        args->opts.stream = true;
        break;
      default:
        die("Parsing error (invalid suboption)\n");
    }
  }
	
  bool optional = false;
  while(*fmt) {
    if(*fmt == '?') {
      optional = true;
      fmt++;
      continue;
    }

    if(optind >= argc) {
      if(!optional)
        die("Too few arguments\n");
	
      break;
    }

    switch(*(fmt++)) {
      case 'n':
        args->pos.depth = parse_long_arg(argv[optind++]);
	break;
      case 'p':
        args->pos.path = strdup(argv[optind++]);
	break;
    }
  }
}

static void parse_subcmd(struct dls_args * args, int argc, char * argv[]) {
  switch(args->cmd){
    case SFM_POP:
      parse_fmt_subcmd("?np", args, argc, argv);
      break;
    case SFM_PUSH:
      parse_fmt_subcmd("p", args, argc, argv);
      break;
    case SFM_TOP:
    case SFM_DISCARD:
      parse_fmt_subcmd("?n", args, argc, argv);
      break;
    default:
      break;
  }
}

struct dls_args * parse_dls_args(int argc, char * argv[]) {
  struct dls_args * args = malloc(sizeof(struct dls_args));

  memset(args, 0, sizeof(struct dls_args));
  args->pos.depth = 1;

  // Parse direct push target is present
  char * base = basename(argv[0]);

  if(strcmp(base, "dls_push") == 0) {
    if(argc < 2)
      die("Please specify input paths\n");

    args->cmd = SFM_PUSH;

    args->dpush.paths = (const char * const *) (&argv[1]);
    args->dpush.size = argc - 1;

    return args;
  }


  int choice;
  while(true) {
    static struct option long_options[] = {
      {"version", no_argument,       0, 'v'},
      {"help",	  optional_argument, 0, 'h'},
      {0,         0,                 0,  0 }
    };
 
    int option_index = 0;

    choice = getopt_long(argc, argv, "+vh::", long_options, &option_index);
  
    if(choice == -1)
      break;
  
    switch(choice) {
      case 'v':
        args->version = true;
        break;
      case 'h':
        args->help = true;
        args->cmd = parse_command(optarg);
        break;
      default:
        die("Parsing error\n");
    }
  }

  if(optind < argc && !args->cmd) {
    args->cmd = parse_command(argv[optind++]); 

    if(args->cmd == SFM_UNKNOWN)
      die("Unknown command %s\n", argv[--optind]);
  }

  if(args->help && optind < argc) 
    die("Too many arguments.\n"); 
 
  int remaining_argc = argc - optind + 1;
  char ** remaining_argv = &argv[optind - 1];

  parse_subcmd(args, remaining_argc, remaining_argv);

  return args;
}

void print_version() {
  printf("Version: " SFM_VERSION "\n");
}

void print_help(struct dls_args * args) {
  dls_help(args->cmd);
}
