#include "extlibs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

static void handle_error(const char * prefix, const char * fmt, int use_errno, va_list ap) {
  char msg[16384];
  char * end = msg + sizeof(msg);
  size_t plen = strlen(prefix);

  if(plen >= sizeof(msg))
    goto err_err;
 
  strncpy(msg, prefix, plen);

  char * remaining = msg + plen;
  int written;
  if((written = vsnprintf(remaining, end - remaining, fmt, ap)) < 0)
    goto err_err;
  
  remaining += written;
    
  if(use_errno) {
    const char * err = errno > 0 && errno <= 133 ? strerror(errno) : "Unknown error";

    if((written = snprintf(remaining, end - remaining, " [%s]\n", err)) < 0)
      goto err_err;
  }

  fputs(msg, stderr);
  fflush(stderr);

  return;

  err_err:
    fputs(msg, stderr);
    fputs("BUG: error handler exceeds maximum buffer length -- partial write\n", stderr);

    fflush(stderr);
    abort();
}

#define VA_ARGS_WRAPPER(prefix, fmt, use_errno)    \
  va_list arg_list;                                \
                                                   \
  va_start(arg_list, fmt);                         \
  handle_error(prefix, fmt, use_errno, arg_list);  \
  va_end(arg_list);

#define DIE_PREFIX "Unrecoverable: "

void NORETURN die(const char * err, ...) {
  VA_ARGS_WRAPPER(DIE_PREFIX, err, false); 
  exit(1);
}


void NORETURN die_errno(const char * err, ...) {
  VA_ARGS_WRAPPER(DIE_PREFIX, err, true);
  exit(1);
}

#define ERR_PREFIX "Error: "

void err(const char * err, ...) {
  VA_ARGS_WRAPPER(ERR_PREFIX, err,  false);
}

void err_errno(const char * err, ...) {
  // save errno
  int errno_ = errno;
  VA_ARGS_WRAPPER(ERR_PREFIX, err, false);
  errno = errno_;
}

#define WARN_PREFIX "Warning: "
void warn(const char * warn, ...) {
  VA_ARGS_WRAPPER(WARN_PREFIX, warn, false);
}

void warn_errno(const char * warn, ...) {
  int errno_ = errno;
  VA_ARGS_WRAPPER(WARN_PREFIX, warn, true);
  errno = errno_; 
}

#define USAGE_PREFIX "Usage: "

void usage(const char * use, ...) {
  VA_ARGS_WRAPPER(USAGE_PREFIX, use, false);
  exit(128);
}
