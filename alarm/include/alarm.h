#ifndef ALARM_H
#define ALARM_H

#define MILLIS_IN_HOUR 3600000

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

class AlarmClock; // fully defined in AlarmClock.h
extern AlarmClock alarmClock;

// defines modes that the clock can be in
enum ClockMode {DISPLAY_TIME, SET_TIME, DISPLAY_ALARM, SET_ALARM};

// defines submodes for SET_TIME and SET_ALARM
enum ClockSetMode {SET_SECOND, SET_MINUTE, SET_HOUR, SET_YEAR, SET_DAY, SET_MONTH};


#endif

