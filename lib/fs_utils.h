#ifndef __UTILS_H
#define __UTILS_H

#include <unistd.h>
#include <sys/stat.h>

#define SWAP(x, y) ({   \
  typeof(x) _x = x;    \
  typeof(y) _y = y;    \
  x = _y;              \
  y = _x;              \
})

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

char * pathcat(const char * prefix, const char * suffix);

int write_all(int fd, void * bytes, size_t size);

int open_or_die(const char * path, int flags);

int open_creat_or_die(const char * path, int flags, mode_t mode);

void fs_move_file(const char * src, const char * dst, const size_t buf_size);

void fs_copy_file(const char * src, const char * dst, const size_t buf_size);

void fd_copy_all(int src_fd, int dst_fd, const size_t buf_size);

void unlink_or_die(const char * path);

#endif
