#ifndef LOGGING_H
#define LOGGING_H

#include "timing.h"


String the_log = "";

void Log(String to_log) {
  the_log += get_time_string();
  the_log += ": " + to_log + "\n";
}


#endif

