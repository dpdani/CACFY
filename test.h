#ifndef TEST_H
#define TEST_H

#include "timing.h"

String test_RTC() {
  if(rtc.now().date() > 31) {
    Log(String("RTC failure. Got date: ") + rtc.now().date());
    return String("Clock failure.");
  }
  return String("");
}



String[] run_tests() {
  boolean fine = true;
  Log("Running system tests.");
  String result = test_RTC();
  if(result != String(""))
    fine = false;
  // ... other tests here ...
  if(fine)
    Log("Tests ran ok.");

  return String[];
}



#endif
