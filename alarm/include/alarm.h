#ifndef ALARM_H
#define ALARM_H

#define MILLIS_IN_HOUR 3600000

// Definitions for LCD pins
#define LCD_ENABLE 2
#define LCD_RS     4
#define LCD_D4    10
#define LCD_D5    11
#define LCD_D6    12
#define LCD_D7    13

// pin (digital read) that controls how bright the LCD display should be
#define LCD_BRIGHTNESS_PIN 9

// push button pin (digital read) that switches between clock modes
#define MODE_PIN 7

// push button pins (analog in) that are used to set the time and alarm
#define SET_TOGGLE_PIN 0
#define SET_PLUS_PIN   1
#define SET_MINUS_PIN  2

// defines how bright the LCD should be (valid values are 0-1023.)
#define LCD_BRIGHTNESS_STEP      50
#define LCD_BRIGHTNESS_DEFAULT   50

// The RTC only deals with years >= 2000 and <= 2099
#define MIN_YEAR 2000
#define MAX_YEAR 2099

class AlarmClock; // fully defined in AlarmClock.h
extern AlarmClock alarmClock;

// defines modes that the clock can be in
enum ClockMode {DISPLAY_TIME, SET_TIME, DISPLAY_ALARM, SET_ALARM};

// defines submodes for SET_TIME and SET_ALARM
enum ClockSetMode {SET_SECOND, SET_MINUTE, SET_HOUR, SET_YEAR, SET_DAY, SET_MONTH};


#endif

