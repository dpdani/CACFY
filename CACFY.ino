#include "timing.h"
#include "logging.h"
#include "cli.h"
#include "lcd.h"


#define PIN_MIN_CISTERNA 30
#define PIN_MIN_POZZO_1 31
#define PIN_MIN_POZZO_2 32
#define PIN_MIN_POZZO_3 33
#define PIN_MAX_CISTERNA 35
#define PIN_MAX_POZZO_1 36
#define PIN_MAX_POZZO_2 37
#define PIN_MAX_POZZO_3 38
#define PIN_CONTROL_POZZO_1 41
#define PIN_CONTROL_POZZO_2 42
#define PIN_CONTROL_POZZO_3 43
#define PIN_ENABLE_CONSOLE 50

#define MINUTES_BEFORE_POZZO_CLOSE 1


enum Level {
  Min,
  Ok,
  Max
};

enum Pozzo {
  Pozzo1,
  Pozzo2,
  Pozzo3
};

enum State {
  Idle,
  Filling
};


Level pozzo_1 = Min;
Level pozzo_2 = Min;
Level pozzo_3 = Min;
Level cisterna = Min;
Pozzo last_pozzo_used = Pozzo3;
State current_state = Idle;


Level get_level_cisterna() {
  if(digitalRead(PIN_MAX_CISTERNA) == LOW && digitalRead(PIN_MIN_CISTERNA) == LOW)
    return Min;
  if(digitalRead(PIN_MAX_CISTERNA) == HIGH && digitalRead(PIN_MIN_CISTERNA) == HIGH)
    return Max;
  return Ok;
}

Level get_level_pozzo_1() {
  if(digitalRead(PIN_MAX_POZZO_1) == LOW && digitalRead(PIN_MIN_POZZO_1) == LOW)
    return Min;
  if(digitalRead(PIN_MAX_POZZO_1) == HIGH && digitalRead(PIN_MAX_POZZO_1) == HIGH)
    return Max;
  return Ok;
}

Level get_level_pozzo_2() {
  if(digitalRead(PIN_MAX_POZZO_2) == LOW && digitalRead(PIN_MIN_POZZO_2) == LOW)
    return Min;
  if(digitalRead(PIN_MAX_POZZO_2) == HIGH && digitalRead(PIN_MAX_POZZO_2) == HIGH)
    return Max;
  return Ok;
}

Level get_level_pozzo_3() {
  if(digitalRead(PIN_MAX_POZZO_3) == LOW && digitalRead(PIN_MIN_POZZO_3) == LOW)
    return Min;
  if(digitalRead(PIN_MAX_POZZO_3) == HIGH && digitalRead(PIN_MAX_POZZO_3) == HIGH)
    return Max;
  return Ok;
}

void open_pozzo_1() {
  Log("Opening pozzo 1.");
  digitalWrite(PIN_CONTROL_POZZO_1, HIGH);
}

void open_pozzo_2() {
  Log("Opening pozzo 2.");
  digitalWrite(PIN_CONTROL_POZZO_2, HIGH);
}

void open_pozzo_3() {
  Log("Opening pozzo 3.");
  digitalWrite(PIN_CONTROL_POZZO_3, HIGH);
}

void close_pozzo_1() {
  Log("Closing pozzo 1.");
  digitalWrite(PIN_CONTROL_POZZO_1, LOW);
}

void close_pozzo_2() {
  Log("Closing pozzo 2.");
  digitalWrite(PIN_CONTROL_POZZO_2, LOW);
}

void close_pozzo_3() {
  Log("Closing pozzo 3.");
  digitalWrite(PIN_CONTROL_POZZO_3, LOW);
}

void close_everything() {
  close_pozzo_1();
  close_pozzo_2();
  close_pozzo_3();
}


void set_state_idle() {
  current_state = Idle;
}


#include "taskmgr.h"


void setup() {
  lcd.begin(16,2);
  lcd.setCursor(0, 0);
  lcd.print("System init...");
  pinMode(PIN_MIN_CISTERNA, INPUT);
  pinMode(PIN_MIN_POZZO_1, INPUT);
  pinMode(PIN_MIN_POZZO_2, INPUT);
  pinMode(PIN_MIN_POZZO_3, INPUT);
  pinMode(PIN_MAX_CISTERNA, INPUT);
  pinMode(PIN_MAX_POZZO_1, INPUT);
  pinMode(PIN_MAX_POZZO_2, INPUT);
  pinMode(PIN_MAX_POZZO_3, INPUT);
  pinMode(PIN_CONTROL_POZZO_1, OUTPUT);
  pinMode(PIN_CONTROL_POZZO_2, OUTPUT);
  pinMode(PIN_CONTROL_POZZO_3, OUTPUT);
  pinMode(PIN_ENABLE_CONSOLE, INPUT);
  Serial.begin(115200);
  Wire.begin();
  rtc.begin();
  last_lcd_idle_update = rtc.now() - TimeSpan(1,0,0,0);  // initial last update set to yesterday
  setSyncProvider(syncProvider);
  taskmgr_init();
  Log("Device started.");
  lcd_idle();
}


void loop() {
  // Open console mode on request
  if(digitalRead(PIN_ENABLE_CONSOLE) == HIGH) {
    Log("Starting console mode.");
    close_everything();
    set_state_idle();
    startConsoleMode();
    lcd_status_string = "OK";  // Console mode quitted.
  }
  // Update LCD
  lcd_idle();
  // Update water levels
  cisterna = get_level_cisterna();
  pozzo_1 = get_level_pozzo_1();
  pozzo_2 = get_level_pozzo_2();
  pozzo_3 = get_level_pozzo_3();
  // Security check
  if(current_state == Filling && cisterna == Max) {
    close_everything();
    set_state_idle();
    taskmgr_init();  // Re-initialize taskmrg to flush previous tasks
  }
  // Start filling
  if(cisterna != Max && current_state == Idle) {
    // if cisterna isn't filled and the we're not already filling it
    if(last_pozzo_used == Pozzo1) {
      open_pozzo_2();
      current_state = Filling;
      last_pozzo_used = Pozzo2;
//      DateTime when = rtc.now() + TimeSpan(0,0, MINUTES_BEFORE_POZZO_CLOSE, 0);
      DateTime when = rtc.now() + TimeSpan(0,0, 0, 10);
      taskmgr_add_task(Close_Pozzo2, when);
      taskmgr_add_task(SetIdle, when);
    }
    else if(last_pozzo_used == Pozzo2) {
      open_pozzo_3();
      current_state = Filling;
      last_pozzo_used = Pozzo3;
//      DateTime when = rtc.now() + TimeSpan(0,0, MINUTES_BEFORE_POZZO_CLOSE, 0);/
      DateTime when = rtc.now() + TimeSpan(0,0, 0, 10);
      taskmgr_add_task(Close_Pozzo3, when);
      taskmgr_add_task(SetIdle, when);
    }
    else if(last_pozzo_used == Pozzo3) {
      open_pozzo_1();
      current_state = Filling;
      last_pozzo_used = Pozzo1;
//      DateTime when = rtc.now() + TimeSpan(0,0, MINUTES_BEFORE_POZZO_CLOSE, 0);/
      DateTime when = rtc.now() + TimeSpan(0,0, 0, 10);
      taskmgr_add_task(Close_Pozzo1, when);
      taskmgr_add_task(SetIdle, when);
    }
  }
  // Look for tasks to be done
  taskmgr_tick();
}

