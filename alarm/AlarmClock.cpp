#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Time.h>

#include "include/AlarmClock.h"


/////////////////////
//  Constructors   //
/////////////////////

AlarmClock::AlarmClock() {

	// default device mode
	mode = DISPLAY_TIME;

	// default brightness of the LCD
	LCDBrightness = LCD_BRIGHTNESS_DEFAULT;

	// setup controls
	pinMode(MODE_PIN, INPUT);
}

/******************************************************************************/

/////////////////////
// Private Methods //
/////////////////////

void AlarmClock::updateBrightness() {

	analogWrite(LCD_BRIGHTNESS_PIN, LCDBrightness);
}

/******************************************************************************/

void AlarmClock::displayTime() {

	int hr   = hour();
	int min  = minute();
	int sec  = second();

	int mnth = month();
	int dy   = day();
	int yr   = year();

	const char *timeMode = hr < 12 ? "AM" : "PM";

	// Midnight is a special case in AM/PM format
	if (0 == hr) {
		hr = 12;
	}

	String hrStr  = hr  < 10 ? "0" + String(hr)  : String(hr);
	String minStr = min < 10 ? "0" + String(min) : String(min);
	String secStr = sec < 10 ? "0" + String(sec) : String(sec);

	String dyStr   = dy < 10 ? "0" + String(dy) : String(dy);

	lcd->setCursor(0, 0);
	lcd->print(String(monthNames[mnth - 1]) + " " + dyStr + ", " + String(yr));
	lcd->setCursor(0, 1);
	lcd->print(hrStr + ":" + minStr + ":" + secStr + " " + timeMode);
}

/******************************************************************************/

void AlarmClock::displaySetTime() {

	// TODO: eventually calls writeRTC() to set time
	lcd->setCursor(0, 0);
	lcd->print("TODO: Set Time");

	return;
}

/******************************************************************************/

void AlarmClock::displayAlarm() {

	// TODO
	lcd->setCursor(0, 0);
	lcd->print("TODO: Show Alarm");

	return;
}

/******************************************************************************/

void AlarmClock::displaySetAlarm() {

	// TODO
	lcd->setCursor(0, 0);
	lcd->print("TODO: Set Alarm");

	return;
}

/******************************************************************************/

/////////////////////
// Public Methods  //
/////////////////////

void AlarmClock::setMode(ClockMode newMode) {

	mode = newMode;
	lcd->clear();
	delay(250);
}

/******************************************************************************/

void AlarmClock::readRTC() {

	// TODO 1: read time from RTC.
	// TODO 2: call setTime() (from Time library) to set that value locally.
	setTime(0, 0, 0, 1, 1, 15); // January 1, 2015, 00:00:00
	return;
}

/******************************************************************************/

void AlarmClock::writeRTC(int hr, int min, int sec, int dy, int mnth, int yr) {

	// TODO: write value to the RTC
	return;
}

/******************************************************************************/

void AlarmClock::initLCD() {

	lcd = new LiquidCrystal(12, 11, 5, 4, 3, 2);
	lcd->begin(16, 2);

	return;
}

/******************************************************************************/

void AlarmClock::updateDisplay() {

	// used to track changes to the mode pin
	static int lastModePinState = LOW;

	// determines how bright the LCD is
	updateBrightness();

	// If user pressed the mode select button, cycle to the next mode.
	int modePinState = digitalRead(MODE_PIN);

	if (modePinState != lastModePinState && HIGH == modePinState) {
		if (SET_ALARM == mode) {
			// At the end, wrap around again to the first value.
			setMode(DISPLAY_TIME);
		} else {
			// There's no ++ operator for enums... Really, C++?
			setMode((ClockMode)((int)mode + 1));
		}
	}

	lastModePinState = modePinState;

	switch (mode) {

		case DISPLAY_TIME:
			displayTime();
			break;

		case SET_TIME:
			displaySetTime();
			break;

		case DISPLAY_ALARM:
			displayAlarm();
			break;

		case SET_ALARM:
			displaySetAlarm();
			break;

		default:
			break;
	}
}

