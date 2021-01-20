#include "left_pad.h"
#include <string.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void leftpad(char * buf, size_t size, char * str) {
  size_t len = strlen(str);
  len = MIN(len, size);

  size_t i;
  for (i = 0; i < size - 1; ++i) {
    if(i < len)
     buf[i] = str[i];
    else
     buf[i] = ' ';
  }

  buf[i] = '\0';
}
