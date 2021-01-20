#ifndef __TIME_DELTA_H
#define __TIME_DELTA_H

#include <time.h>

struct time_delta {
  int secs;
  int mins;
  int hours;
  int days;
};

void get_time_delta(struct time_delta * delta, time_t time);

#endif /* ifndef __TIME_DELTA_H */
