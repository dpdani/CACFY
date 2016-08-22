#ifndef TIMING_H
#define TIMING_H

#include <Wire.h>
#include "RTClib.h"
#include <Time.h>
#include <TimeAlarms.h>

// Using a DS3231
RTC_DS3231 rtc;



uint32_t syncProvider() {
  rtc.now().unixtime();
}


String get_time_string() {
  DateTime t = rtc.now();
  String str = "";
  if(t.day() < 10)
    str += String("0") + t.day();
  else
    str += t.day();
  str += "/";
  if(t.month() < 10)
    str += String("0") + t.month();
  else
    str += t.month();
  str += "/";
  str += t.year();
  str += "-";
  if(t.hour() < 10)
    str += String("0") + t.hour();
  else
    str += t.hour();
  str += ":";
  if(t.minute() < 10)
    str += String("0") + t.minute();
  else
    str += t.minute();
  str += ":";
  if(t.second() < 10)
    str += String("0") + t.second();
  else
    str += t.second();
  return str;
}

String get_lcd_time_string() {
  DateTime t = rtc.now();
  String str = "";
  if(t.day() < 10)
    str += String("0") + t.day();
  else
    str += t.day();
  str += "/";
  if(t.month() < 10)
    str += String("0") + t.month();
  else
    str += t.month();
  str += "/";
  str += t.year();
  str += " ";
  if(t.hour() < 10)
    str += String("0") + t.hour();
  else
    str += t.hour();
  str += ":";
  if(t.minute() < 10)
    str += String("0") + t.minute();
  else
    str += t.minute();
  return str;
}


#endif

