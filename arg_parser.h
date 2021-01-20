#ifndef __ARG_PARSER
#define __ARG_PARSER
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_DEPTH ULONG_MAX

enum dls_cmd {
  SFM_UNKNOWN,
  SFM_POP,
  SFM_LIST,
  SFM_TOP,
  SFM_DISCARD,
  SFM_PUSH
};

static char * dls_cmd_str [] = {
  [SFM_UNKNOWN] = "unknown",
  [SFM_POP]     = "pop",
  [SFM_LIST]    = "list",
  [SFM_TOP]     = "top",
  [SFM_DISCARD] = "discard",
  [SFM_PUSH]    = "push",
};

struct dls_args {
  bool help;
  bool version;

  enum dls_cmd cmd;

  struct {
    const char * const * paths;
    size_t size;
  } dpush;

  struct {
    bool stream;
  } opts;

  struct {
    unsigned long depth;
    char * path;
  } pos;
};

struct dls_args * parse_dls_args(int argc, char * argv[]);

void print_version();
void print_help();

#endif
