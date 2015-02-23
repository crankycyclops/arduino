#ifndef ALARM_H
#define ALARM_H

// pin that controls how bright the LCD display should be
#define LCD_BRIGHTNESS_PIN 9

// defines how bright the LCD should be (valid values are 0-1023.)
#define LCD_BRIGHTNESS_STEP      50
#define LCD_BRIGHTNESS_DEFAULT  125

// defines modes that the clock can be in
enum ClockMode {DISPLAY_TIME, SET_TIME, DISPLAY_ALARM, SET_ALARM};

// list of months (0-indexed)
extern const char *monthNames[];


#endif

