#ifndef LOGGING_H
#define LOGGING_H

#include "timing.h"


#define LOG_MAX_SIZE 2000 // in number of characters

String the_log = "";

void Log(String to_log) {
  the_log += get_time_string();
  the_log += ": " + to_log + "\n";
  int log_length = the_log.length();
  if(log_length > LOG_MAX_SIZE)  // Implement rotary behaviour of the log
    the_log = the_log.substring(log_length - LOG_MAX_SIZE, log_length);
}


#endif

