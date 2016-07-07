#ifndef TIMING_H
#define TIMING_H

#include <Wire.h>
#include "Sodaq_DS3231.h"

// Using a DS3231


String get_time_string() {
  DateTime t = rtc.now();
  return String(t.date()) + "/" + t.month() + "/" + t.year() + "-" + t.hour() + ":" + t.minute() + ":" + t.second();
}


#endif
