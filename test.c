#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[]) {
  if(argc != 2)
   return 1;

  int fd = open(argv[1], O_RDWR);
  

  const struct timespec ts[2] = {
    {1000, 1000000},
    {1000, 1000000}
  };
  
  if(-1 == futimens(fd, ts)) {
    printf("%d\n", errno);
    return 2;
  }

  return 0;
}
