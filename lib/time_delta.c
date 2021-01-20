#include "time_delta.h"

#define MINUTES_SEC 60
#define HOURS_SEC (60 * MINUTES_SEC)
#define DAYS_SEC (24 * HOURS_SEC)

static void inline break_into_increment(int * location, time_t increment, time_t * time) {
  *location = *time / increment;
  *time = *time % increment; 
}

void get_time_delta(struct time_delta * delta, time_t time) {
  break_into_increment(&delta->days,  DAYS_SEC,    &time);
  break_into_increment(&delta->hours, HOURS_SEC,   &time);
  break_into_increment(&delta->mins,  MINUTES_SEC, &time);
  delta->secs = time;
}
