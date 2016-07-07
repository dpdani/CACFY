#include "timing.h"
#include "logging.h"
#include "cli.h"



void setup() {
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  Log("Device started.");
}


void loop() {
  if(Serial) {
    Log("Serial connected.");
    startConsoleMode();
  }
}
