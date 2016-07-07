#ifndef TIMING_H
#define TIMING_H

#include <Time.h>


String get_time_string() {
  time_t t = now();
  return String(day(t)) + "/" + month(t) + "/" + year(t) + "-" + hour(t) + ":" + minute(t) + ":" + second(t);
}


#endif
