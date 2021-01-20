#include "utils.h"
#include "lib/extlibs.h"

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>

int open_or_die(const char * path, int flags) {
  int fd = open(path, flags);

  if(fd < 0)
    die_errno("Could not open %s", path);

  return fd;
}

int open_creat_or_die(const char * path, int flags, mode_t mode) {
  int fd = open(path, flags, mode);

  if(fd < 0)
    die_errno("Could not open %s", path);

  return fd;
}

char * pathcat(const char * prefix, const char * suffix) {
  size_t len = strlen(prefix) + 1 + strlen(suffix);
  char * path = malloc(len + 1);

  if(path) {
    strcpy(path, prefix);
    strcat(path, "/");
    strcat(path, suffix);
  }
 
  return path; 
}

void unlink_or_die(const char * path) {
  if(-1 == unlink(path))
    die("Failed to unlink %s", path);
}

int write_all(int fd, void * bytes, size_t size) {
  ssize_t nw;

  do {
    nw = write(fd, bytes, size);

    if(nw < 0)
      return -1;

    size -= nw;
  } while(size > 0);

  return 0;
}

void fd_copy_all(int src_fd, int dst_fd, const size_t buf_size) {
  uint8_t * bytes = alloca(buf_size);
  
  ssize_t nr;

  while(true) {
    nr = read(src_fd, bytes, buf_size);

    if(!nr)
      break; // success
    else if(nr < 0)
      goto io_err;

    if(write_all(dst_fd, bytes, nr) == -1)
      goto io_err;
  }

  return;

  io_err:
    die("An IO error occurred while copying %d to %d", src_fd, dst_fd);
}

void fs_copy_file(const char * src, const char * dst, const size_t buf_size) {
  int src_fd = open_or_die(src, O_RDONLY);
  
  struct stat s;
  if(-1 == fstat(src_fd, &s)) 
    goto err_open_src;
 
  int dst_fd = open_creat_or_die(dst, O_WRONLY | O_CREAT, s.st_mode);
  
  if(dst_fd < 0)
    goto err_open_dst;

  // copy ownership
  if(-1 == fchown(dst_fd, s.st_uid, s.st_gid))
    goto err_copy_attributes;
 
  // copy timestamp 
  struct timespec time_stamp[2] = {
    s.st_atim,
    s.st_mtim
  };
  
  if(-1 == futimens(dst_fd, time_stamp))
    goto err_copy_attributes;

  // copy data
  fd_copy_all(src_fd, dst_fd, buf_size);

  return;

  err_open_src:
  die("An error occurred while opening %s", src);
  err_open_dst:
  die("An error occurred while opening %s", dst);
  err_copy_attributes:
  die("An error occurred while copying attributes to %s", dst);
}


void fs_move_file(const char * src_path, const char * dst_path, size_t buf_size) {
  if(-1 == rename(src_path, dst_path)) {

    // Attempt to relink the file, fail if linking across file systems
    if(errno != EXDEV)
      die_errno("Cannot rename file");

    errno = 0;
    
    fs_copy_file(src_path, dst_path, buf_size);

    if(-1 == unlink(src_path))
      die("Cannot remove %s", src_path);
  }
}
