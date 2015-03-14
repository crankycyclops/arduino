#ifndef ALARMCLOCK_H
#define ALARMCLOCK_H

#include <LiquidCrystal.h>
#include <TimeAlarms.h>

#include "include/alarm.h"


// Alarm payload
void triggerAlarm();

/******************************************************************************/

class AlarmClock {

	private:

		// Current mode of the alarm clock.
		ClockMode mode;

		// The brightness of the LCD.
		int LCDBrightness;

		// Initialize the library with the numbers of the interface pins.
		LiquidCrystal *lcd;

		// Points to an alarm object.
		AlarmID_t alarmID;

		// Whether or not the alarm is enabled.
		bool alarmEnabled;

		// Current and last states of the mode toggle button.
		int modePinState;
		int lastModePinState;

		// Current and last states of the set time/alarm toggle button.
		int setToggleState;
		int lastSetToggleState;

		// Enables and disables the alarm.
		void toggleAlarm(bool enable);

		// Checks state of the mode toggle button and changes the clock's
		// mode if it's pressed. Returns true if the button was pressed and
		// false otherwise.
		bool checkModeToggle();

		// Updates the brightness of the LCD. Called by updateDisplay().
		void updateBrightness();

		// Prints the specified time part (second, minute, etc.) to the LCD
		// display in the appropriate position and in the correct format.
		void printTimePart(ClockSetMode part, int val);

		// Prompts the user for the specified time part (second, minute, etc.)
		// and returns the result.
		int promptTimePart(int initVal, ClockSetMode part, int min, int max);

		// Displays the given time on the LCD.
		void printTime(int hr, int min, int sec, int mnth, int dy, int yr, bool showDate = true);

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

		// Syncs current time from the RTC.
		void readRTC();

		// Writes specified time to the RTC.
		void writeRTC(time_t time);

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

