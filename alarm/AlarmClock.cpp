#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Time.h>
#include <TimeAlarms.h>
#include <Wire.h>
#include <DS1307RTC.h>

#include "include/utility.h"
#include "include/AlarmClock.h"


// This gets called whenever the alarm goes off.
void triggerAlarm() {

	// TODO
	return;
}

/******************************************************************************/


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

	// setup the alarm
	alarmID = Alarm.alarmRepeat(0, 0, 0, triggerAlarm);
	toggleAlarm(false);
}

/******************************************************************************/

/////////////////////
// Private Methods //
/////////////////////

void AlarmClock::toggleAlarm(bool enable) {

	if (enable) {
		Alarm.enable(alarmID);
		alarmEnabled = true;
	}

	else {
		Alarm.disable(alarmID);
		alarmEnabled = false;
	}
}

/******************************************************************************/

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

			val = promptTimePart(year(), curState, MIN_YEAR, MAX_YEAR);

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

	// sync to the RTC when we're done setting the time
	if (SET_TIME == mode) {
		writeRTC(now());
	}

	return;
}

/******************************************************************************/

void AlarmClock::displayAlarm() {

	time_t alarmTime = Alarm.read(alarmID);

	printTime(hour(alarmTime), minute(alarmTime), second(alarmTime), 0, 0, 0, false);

	lcd->setCursor(0, 0);
	lcd->print(alarmEnabled ? "Alarm is ON" : "Alarm is OFF");

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

	DS1307RTC *rtc = new DS1307RTC();
	time_t time = rtc->get();

	setTime(hour(time), minute(time), second(time), day(time), month(time), year(time));
	return;
}

/******************************************************************************/

void AlarmClock::writeRTC(time_t time) {

	DS1307RTC *rtc = new DS1307RTC();

	rtc->set(time);
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

