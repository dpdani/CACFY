#ifndef LOGGING_H
#define LOGGING_H

#include "timing.h"


String the_log = "";

void Log(String to_log) {
  the_log += "LOG ENTRY----------------- ";
  the_log += get_time_string();
  the_log += "\n" + to_log + "\n\n";
}


#endif
