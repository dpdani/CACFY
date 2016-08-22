#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal.h>
#include "timing.h"

LiquidCrystal lcd(8,9,4,5,6,7);

String lcd_status_string = "OK";  // Global status string.
DateTime last_lcd_idle_update;

String lcd_current_string_top = "";
String lcd_current_string_bottom = "";


void lcd_write(String str_top, String str_bottom) {
  if(str_top != lcd_current_string_top || str_bottom != lcd_current_string_bottom) {
    lcd.clear();
    lcd.home();
    lcd.print(str_top);
    lcd.setCursor(0,1);
    lcd.print(str_bottom);
    lcd_current_string_top = str_top;
    lcd_current_string_bottom = str_bottom;
  }
}


void lcd_idle() {
  // TODO: automatic refresh.
  // Serial.println((rtc.now() - last_lcd_idle_update - TimeSpan(0,0,0,10)).seconds());
  if((rtc.now() - last_lcd_idle_update).seconds() < 1)  // don't update LCD unless 1 second passed since last update
    return;
  lcd_write(get_lcd_time_string(), lcd_status_string);
  last_lcd_idle_update = rtc.now();
}

void INT0_ISR() {
  lcd_idle();
}



#endif

