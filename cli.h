#ifndef CLI_H
#define CLI_H

#include "logging.h"
#include "timing.h"



void startConsoleMode() {
  // TODO: write out to LCD
  delay(100);
  Serial.write("CONSOLE MODE\n\n");
  while(Serial) {
    Serial.write("Password: ");
    String password = readFromConsole();
    Serial.println(password);
    if(password == "woll") {
      Serial.write("Access granted.\n");
      break;
    }
    else {
      Log(String("Wrong password entered: ") + password);
      Serial.write("Incorrect password.\n");
    }
  }
  Log("Console opened");
  Serial.write("Console is opened. Check the 'help' command in need.\nTo close the console unplug the device.\n\n");
  while(Serial) {
    Serial.write("$ ");
    String command = readFromConsole();
    if (command.length() > 0) {
      Serial.println(command);
      executeCommand(command);
    } else Serial.write("\n");
  }
  Log("Console closed.");
}

void executeCommand(String command) {
  if(command == "help") {
    Serial.write("HELP GUIDE.\n\nList of commands:\n - help: shows this message.\n - show_analog: shows the value read on analog inputs.\n - show_log: shows the entire log.\n");
    Serial.write(" - set_time: sets internal system time.\n");
  }
  else if (command == "show_analog") {
    Serial.write("0 -> 0V // 1024 -> 5V\n");
    Serial.write(" - A0 : "); Serial.print(analogRead(A0 )); Serial.write("\n");
    Serial.write(" - A1 : "); Serial.print(analogRead(A1 )); Serial.write("\n");
    Serial.write(" - A2 : "); Serial.print(analogRead(A2 )); Serial.write("\n");
    Serial.write(" - A3 : "); Serial.print(analogRead(A3 )); Serial.write("\n");
    Serial.write(" - A4 : "); Serial.print(analogRead(A4 )); Serial.write("\n");
    Serial.write(" - A5 : "); Serial.print(analogRead(A5 )); Serial.write("\n");
    Serial.write(" - A6 : "); Serial.print(analogRead(A6 )); Serial.write("\n");
    Serial.write(" - A7 : "); Serial.print(analogRead(A7 )); Serial.write("\n");
    Serial.write(" - A8 : "); Serial.print(analogRead(A8 )); Serial.write("\n");
    Serial.write(" - A9 : "); Serial.print(analogRead(A9 )); Serial.write("\n");
    Serial.write(" - A10: "); Serial.print(analogRead(A10)); Serial.write("\n");
    Serial.write(" - A11: "); Serial.print(analogRead(A11)); Serial.write("\n");
    Serial.write(" - A12: "); Serial.print(analogRead(A12)); Serial.write("\n");
    Serial.write(" - A13: "); Serial.print(analogRead(A13)); Serial.write("\n");
    Serial.write(" - A14: "); Serial.print(analogRead(A14)); Serial.write("\n");
    Serial.write(" - A15: "); Serial.print(analogRead(A15)); Serial.write("\n");
  }
  else if (command == "show_log") {
    Serial.print(the_log);
    Log("Log view requested.");
  }
  else if (command == "flush_log") {
    the_log = String("");
    Serial.write("Log flushed.\n");
  }
  else if (command == "set_time") {
    Serial.write("System time setting override.\nYear (0 to cancel): ");
    int year = readFromConsole().toInt();
    if (year == 0) {Serial.write("\n"); return;}
    Serial.println(year);
    Serial.write("Month (0 to cancel): ");
    int month = readFromConsole().toInt();
    if (month == 0) {Serial.write("\n"); return;}
    Serial.println(month);
    Serial.write("Day (0 to cancel): ");
    int day = readFromConsole().toInt();
    if (day == 0) {Serial.write("\n"); return;}
    Serial.println(day);
    Serial.write("Cannot cancel from now on.\n");
    Serial.write("Day of week (Sunday -> 0, Monday -> 1, ...): ");
    int dayofweek = readFromConsole().toInt();
    Serial.println(dayofweek);
    Serial.write("Hour: ");
    int hour = readFromConsole().toInt();
    Serial.println(hour);
    Serial.write("Minute: ");
    int minute = readFromConsole().toInt();
    Serial.println(minute);
    Serial.write("Second: ");
    int second = readFromConsole().toInt();
    Serial.println(second);
    DateTime dt(year, month, day, hour, minute, second, dayofweek);
    Log("System time override before.");
    rtc.setDateTime(dt);
    Log("System time override after.");
    Serial.write("Time correctly set.\n");
  }
  else {
    Serial.write("unrecognized_command ");
    Serial.print(command);
    Serial.write("\n");
  }
}

String readFromConsole() {
  String data = "";
  while(1) {
    if(Serial.available() > 0) {
      char c = (char)Serial.read();
      if(c == '\n') {
        return data;
      }
      if (int(c) != 255 || c != '\xff')
        data += c;
    }
  }
}

#endif
