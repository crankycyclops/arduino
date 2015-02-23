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
}

/******************************************************************************/

/////////////////////
// Private Methods //
/////////////////////

void AlarmClock::updateTime(int hr, int min, int sec, int dy, int mnth, int yr) {

	// TODO: also update the RTC's stored time
	setTime(hr, min, sec, dy, mnth, yr);
	return;
}

/******************************************************************************/

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

/////////////////////
// Public Methods  //
/////////////////////

void AlarmClock::setMode(ClockMode newMode) {

	mode = newMode;
	lcd->clear();
}

/******************************************************************************/

void AlarmClock::initClock() {

	// TODO: grab value from RTC
	setTime(0, 0, 0, 1, 1, 15); // January 1, 2015, 00:00:00
	return;
}

/******************************************************************************/

void AlarmClock::initLCD() {

	lcd = new LiquidCrystal(12, 11, 5, 4, 3, 2);
	lcd->begin(16, 2);

	return;
}

/******************************************************************************/

void AlarmClock::setCurrentTime() {

	// TODO
	return;
}

/******************************************************************************/

void AlarmClock::updateDisplay() {

	// determines how bright the LCD is
	updateBrightness();

	switch (mode) {

		case DISPLAY_TIME:
			displayTime();
			break;

		case SET_TIME:
			// TODO
			break;

		case DISPLAY_ALARM:
			// TODO
			break;

		case SET_ALARM:
			// TODO
			break;

		default:
			break;
	}
}

