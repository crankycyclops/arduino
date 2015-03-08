#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Time.h>

#include "include/utility.h"
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

	// initialize the state of the mode button
	modePinState = LOW;
	lastModePinState = LOW;

	// initialize the state of the set time/alarm toggle button
	setToggleState = LOW;
	lastSetToggleState = LOW;
}

/******************************************************************************/

/////////////////////
// Private Methods //
/////////////////////

bool AlarmClock::checkModeToggle() {

	bool state = false;

	if (wasButtonPushed(modePinState, lastModePinState, MODE_PIN)) {

		state = true;

		if (SET_ALARM == mode) {
			// At the end, wrap around again to the first value.
			setMode(DISPLAY_TIME);
		} else {
			// There's no ++ operator for enums... Really, C++?
			setMode((ClockMode)((int)mode + 1));
		}
	}

	return state;
}

/******************************************************************************/

void AlarmClock::updateBrightness() {

	analogWrite(LCD_BRIGHTNESS_PIN, LCDBrightness);
}

/******************************************************************************/

void AlarmClock::printTimePart(ClockSetMode part, int val) {

	switch (part) {

		case SET_SECOND:
			printTime(hour(), minute(), val, month(), day(), year());
			break;

		case SET_MINUTE:
			printTime(hour(), val, second(), month(), day(), year());
			break;

		case SET_HOUR:
			printTime(val, minute(), second(), month(), day(), year());
			break;

		case SET_YEAR:
			printTime(hour(), minute(), second(), month(), day(), val);
			break;

		case SET_MONTH:
			printTime(hour(), minute(), second(), val, day(), year());
			break;

		case SET_DAY:
			printTime(hour(), minute(), second(), month(), val, year());
			break;

		default:
			break;
	}

	return;
}

/******************************************************************************/

int AlarmClock::promptTimePart(int initVal, ClockSetMode part, int min, int max) {

	// Vertical coordinate of the time part on the LCD display.
	static int yLCDPos[] = {1, 1, 1, 0, 0, 0};

	// Horizontal coordinate of the time part on the LCD display.
	static int xLCDPos[] = {6, 3, 0, 8, 4, 0};

	// How many characters are displayed for the time part.
	static int LCDwidth[] = {2, 2, 2, 4, 2, 3};

	// initialize the state of the + button
	int setPlusState = LOW;
	int lastSetPlusState = LOW;

	// initialize the state of the - button
	int setMinusState = LOW;
	int lastSetMinusState = LOW;

	// handles blinking
	bool blank = false;
	unsigned long prevMillisecond = 0;

	int val = initVal;

	while (1) {

		unsigned long curMillisecond = millis();

		// Blinking seconds
		if (curMillisecond - prevMillisecond > 500) {
			prevMillisecond = curMillisecond;
			blank = !blank;
		}

		if (blank) {
			lcd->setCursor(xLCDPos[(int)part], yLCDPos[(int)part]);
			for (int i = 0; i < LCDwidth[(int)part]; i++) {
				lcd->print(" ");
			}
		} else {
			printTimePart(part, val);
		}

		// if mode button is pressed, we need to drop out of SET_TIME
		if (checkModeToggle()) {
			return 0;
		}

		// continue trying to set second until we toggle to the next time part
		else if (wasButtonPushed(setToggleState, lastSetToggleState, SET_TOGGLE_PIN, true)) {
			break;
		}

		// user pressed the + button
		else if (wasButtonPushed(setPlusState, lastSetPlusState, SET_PLUS_PIN, true)) {
			val = val >= max ? min : val + 1;
		}

		// user pressed the - button
		else if (wasButtonPushed(setMinusState, lastSetMinusState, SET_MINUS_PIN, true)) {
			val = val > min ? val - 1 : max;
		}
	}

	return val;
}

/******************************************************************************/

void AlarmClock::printTime(int hr, int min, int sec, int mnth, int dy, int yr, bool showDate) {

	const char *timeMode = hr < 12 ? "AM" : "PM";

	// Midnight is a special case in AM/PM format
	if (0 == hr) {
		hr = 12;
	}

	// We're not displaying military time
	else if (hr > 12) {
		hr -= 12;
	}

	String hrStr  = hr  < 10 ? "0" + String(hr)  : String(hr);
	String minStr = min < 10 ? "0" + String(min) : String(min);
	String secStr = sec < 10 ? "0" + String(sec) : String(sec);

	String dyStr   = dy < 10 ? "0" + String(dy) : String(dy);

	if (showDate) {
		lcd->setCursor(0, 0);
		lcd->print(String(monthShortStr(mnth)) + " " + dyStr + ", " + String(yr));
	}

	lcd->setCursor(0, 1);
	lcd->print(hrStr + ":" + minStr + ":" + secStr + " " + timeMode);

	return;
}

/******************************************************************************/

void AlarmClock::displayTime() {

	printTime(hour(), minute(), second(), month(), day(), year());
}

/******************************************************************************/

void AlarmClock::displaySetTime() {

	int val;
	static ClockSetMode curState = SET_SECOND;

	int monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if (isLeapYear(year())) {
		monthDays[1] = 29;
	}

	switch (curState) {

		case SET_SECOND:

			val = promptTimePart(second(), curState, 0, 59);

			if (SET_TIME == mode) {
				setTime(hour(), minute(), val, day(), month(), year());
			}

			break;

		case SET_MINUTE:

			val = promptTimePart(minute(), curState, 0, 59);

			if (SET_TIME == mode) {
				setTime(hour(), val, second(), day(), month(), year());
			}

			break;

		case SET_HOUR:

			val = promptTimePart(hour(), curState, 0, 23);

			if (SET_TIME == mode) {
				setTime(val, minute(), second(), day(), month(), year());
			}

			break;

		case SET_YEAR:

			val = promptTimePart(year(), curState, 1971, 2999);

			if (SET_TIME == mode) {

				// Do bounds checking for leap years in February
				if (!isLeapYear(val) && 2 == month() && 29 == day()) {
					setTime(hour(), minute(), second(), 1, 3, val);
				} else {
					setTime(hour(), minute(), second(), day(), month(), val);
				}
			}

			break;

		case SET_MONTH:

			val = promptTimePart(month(), curState, 1, 12);

			if (SET_TIME == mode) {

				// Do bounds checking for days in the month
				if (day() > monthDays[val]) {
					setTime(hour(), minute(), second(), monthDays[val], val, year());
				} else {
					setTime(hour(), minute(), second(), day(), val, year());
				}
			}

			break;

		case SET_DAY:

			val = promptTimePart(day(), curState, 1, monthDays[month() - 1]);

			if (SET_TIME == mode) {
				setTime(hour(), minute(), second(), val, month(), year());
			}

			break;

		default:
			break;
	}

	if (SET_MONTH == curState) {
		// At the end, wrap around again to the first value.
		curState = SET_SECOND;
	} else {
		curState = (ClockSetMode)((int)curState + 1);
	}

	// reset when we leave SET_TIME
	if (SET_TIME != mode) {
		curState = SET_SECOND;
	}

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

	return;
}

/******************************************************************************/

void AlarmClock::readRTC() {

	// TODO 1: read time from RTC.
	// TODO 2: call setTime() (from Time library) to set that value locally.
	setTime(0, 0, 0, 1, 1, 15); // January 1, 2015, 01:00:00
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

	// determines how bright the LCD is
	updateBrightness();

	// If user pressed the mode select button, cycle to the next mode.
	checkModeToggle();

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

