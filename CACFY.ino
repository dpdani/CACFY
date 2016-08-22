#include "timing.h"
#include "logging.h"
#include "cli.h"
#include "lcd.h"


#define PIN_LEVEL_CISTERNA A10
#define PIN_LEVEL_POZZO_1 A11
#define PIN_LEVEL_POZZO_2 A12
#define PIN_LEVEL_POZZO_3 A13
#define PIN_CONTROL_POZZO_1 41
#define PIN_CONTROL_POZZO_2 42
#define PIN_CONTROL_POZZO_3 43
#define PIN_CONTROL_ACQUEDOTTO 44


float level_pozzo_1 = 0.0;
float level_pozzo_2 = 0.0;
float level_pozzo_3 = 0.0;
float level_cisterna = 0.0;
float level_cisterna_enough = 0.80;
float level_minimum = 0.0;


void open_pozzo_1() {
  digitalWrite(PIN_CONTROL_POZZO_1, HIGH);
}

void open_pozzo_2() {
  digitalWrite(PIN_CONTROL_POZZO_2, HIGH);
}

void open_pozzo_3() {
  digitalWrite(PIN_CONTROL_POZZO_3, HIGH);
}

void open_acquedotto() {
  digitalWrite(PIN_CONTROL_ACQUEDOTTO, HIGH);
}

void close_pozzo_1() {
  digitalWrite(PIN_CONTROL_POZZO_1, LOW);
}

void close_pozzo_2() {
  digitalWrite(PIN_CONTROL_POZZO_2, LOW);
}

void close_pozzo_3() {
  digitalWrite(PIN_CONTROL_POZZO_3, LOW);
}

void close_acquedotto() {
  digitalWrite(PIN_CONTROL_ACQUEDOTTO, LOW);
}

void close_everything() {
  close_pozzo_1();
  close_pozzo_2();
  close_pozzo_3();
  close_acquedotto();
}


void fill_cisterna() {
  Log("Filling cisterna...");
  lcd_write("Riempio cisterna", "Rilevo livelli");
  level_cisterna = analogRead(PIN_LEVEL_CISTERNA) / 1024.0;
  level_pozzo_1 = analogRead(PIN_LEVEL_POZZO_1) / 1024.0;
  level_pozzo_2 = analogRead(PIN_LEVEL_POZZO_2) / 1024.0;
  level_pozzo_3 = analogRead(PIN_LEVEL_POZZO_3) / 1024.0;
  while(level_cisterna < level_cisterna_enough) {
    float maximum_level = max(level_pozzo_1, level_pozzo_2);  // Using most filled pozzo.
    maximum_level = max(maximum_level, level_pozzo_3);
    if(maximum_level >= level_minimum) {
      if (maximum_level == level_pozzo_1) {
        open_pozzo_1();
        lcd_write("Riempio cisterna", "Uso pozzo 1...");
      }
      else if(maximum_level == level_pozzo_2) {
        open_pozzo_2();
        lcd_write("Riempio cisterna", "Uso pozzo 2...");
      }
      else if(maximum_level == level_pozzo_3) {
        open_pozzo_3();
        lcd_write("Riempio cisterna", "Uso pozzo 3...");
      }
      else {
        Log("Weird behaviour on water level detection. Using pozzo 1.");
        open_pozzo_1();
        lcd_write("Riempio cisterna", "Fallback pozzo 1");
      }
    }
    else {  // Available water level insufficient -> acquedotto
      open_acquedotto();
      lcd_write("Riempio cisterna", "Uso acquedotto...");
    }
    delay(5000);
    close_everything();
  }
  Log("Cisterna filled.");
  lcd_status_string = "OK";
  lcd_idle();
}


void setup() {
  lcd.begin(16,2);
  lcd.setCursor(0, 0);
  lcd.print("System init...");
  pinMode(PIN_LEVEL_CISTERNA, INPUT);
  pinMode(PIN_LEVEL_POZZO_1, INPUT);
  pinMode(PIN_LEVEL_POZZO_2, INPUT);
  pinMode(PIN_LEVEL_POZZO_3, INPUT);
  pinMode(PIN_CONTROL_POZZO_1, OUTPUT);
  pinMode(PIN_CONTROL_POZZO_2, OUTPUT);
  pinMode(PIN_CONTROL_POZZO_3, OUTPUT);
  pinMode(PIN_CONTROL_ACQUEDOTTO, OUTPUT);
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  last_lcd_idle_update = rtc.now() - TimeSpan(1,0,0,0);  // initial last update set to yesterday
  setSyncProvider(syncProvider);
  Log("Device started.");
  lcd_idle();
}


void loop() {
//  if(Serial) {
//    Log("Serial connected.");
//    startConsoleMode();
//  }
  lcd_idle();

  if(true) {
    fill_cisterna();
  }
}

