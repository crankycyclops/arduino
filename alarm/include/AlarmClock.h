#ifndef ALARMCLOCK_H
#define ALARMCLOCK_H

#include <LiquidCrystal.h>
#include "include/alarm.h"


class AlarmClock {

	private:

		// Current mode of the alarm clock.
		ClockMode mode;

		// the brightness of the LCD
		int LCDBrightness;

		// initialize the library with the numbers of the interface pins.
		LiquidCrystal *lcd;

		// Writes the current time to the alarm clock and the RTC. Called by setTime().
		void updateTime(int hr, int min, int sec, int dy, int mnth, int yr);

		// Updates the brightness of the LCD. Called by updateDisplay().
		void updateBrightness();

		// Called by updateDisplay() when mode = DISPLAY_TIME.
		void displayTime();

		// Called by updateDisplay() when mode = SET_TIME.
		void displaySetTime();

		// Called by updateDisplay() when mode = DISPLAY_ALARM.
		void displayAlarm();

		// Called by updateDisplay() when mode = SET_ALARM.
		void displaySetAlarm();

	public:

		// Constructor
		AlarmClock();

		// Sets the alarm clock's mode.
		void setMode(ClockMode newMode);

		// Sets the brightness of the LCD.
		inline void setLCDBrightness(int brightness) {LCDBrightness = brightness;}

		// Returns the alarm clock's mode.
		inline ClockMode getMode() {return mode;}

		// Initializes the clock.
		void initClock();

		// Initializes the LCD.
		void initLCD();

		// This updates the LCD display. Called by each iteration of loop().
		void updateDisplay();
};


#endif

