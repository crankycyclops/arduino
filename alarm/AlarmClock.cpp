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

bool AlarmClock::wasButtonPushed(int &curState, int &lastState, int pin, bool analog) {

	bool pushed = false;

	if (analog) {
		curState = analogRead(pin) ? HIGH : LOW;
	}

	else {
		curState = digitalRead(pin);
	}

	if (curState != lastState && HIGH == curState) {
		pushed = true;
	}

	lastState = curState;

	return pushed;
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

int AlarmClock::promptSecond() {

	// initialize the state of the + button
	int setPlusState = LOW;
	int lastSetPlusState = LOW;

	// initialize the state of the - button
	int setMinusState = LOW;
	int lastSetMinusState = LOW;

	// current value
	int val = second();

	while (1) {

		printTime(hour(), minute(), val, month(), day(), year());

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
			val = val >= 59 ? 0 : val + 1;
		}

		// user pressed the - button
		else if (wasButtonPushed(setMinusState, lastSetMinusState, SET_MINUS_PIN, true)) {
			val = val > 0 ? val - 1 : 59;
		}
	}

	return val;
}

/******************************************************************************/

int AlarmClock::promptMinute() {

	// initialize the state of the + button
	int setPlusState = LOW;
	int lastSetPlusState = LOW;

	// initialize the state of the - button
	int setMinusState = LOW;
	int lastSetMinusState = LOW;

	// current value
	int val = minute();

	while (1) {

		printTime(hour(), val, second(), month(), day(), year());

		// if mode button is pressed, we need to drop out of SET_TIME
		if (checkModeToggle()) {
			return 0;
		}

		// continue trying to set minute until we toggle to the next time part
		else if (wasButtonPushed(setToggleState, lastSetToggleState, SET_TOGGLE_PIN, true)) {
			break;
		}

		// user pressed the + button
		else if (wasButtonPushed(setPlusState, lastSetPlusState, SET_PLUS_PIN, true)) {
			val = val >= 59 ? 0 : val + 1;
		}

		// user pressed the - button
		else if (wasButtonPushed(setMinusState, lastSetMinusState, SET_MINUS_PIN, true)) {
			val = val > 0 ? val - 1 : 59;
		}
	}

	return val;
}

/******************************************************************************/

int AlarmClock::promptHour() {

	// initialize the state of the + button
	int setPlusState = LOW;
	int lastSetPlusState = LOW;

	// initialize the state of the - button
	int setMinusState = LOW;
	int lastSetMinusState = LOW;

	// current value
	int val = hour();

	while (1) {

		printTime(val, minute(), second(), month(), day(), year());

		// if mode button is pressed, we need to drop out of SET_TIME
		if (checkModeToggle()) {
			return 0;
		}

		// continue trying to set hour until we toggle to the next time part
		else if (wasButtonPushed(setToggleState, lastSetToggleState, SET_TOGGLE_PIN, true)) {
			break;
		}

		// user pressed the + button
		else if (wasButtonPushed(setPlusState, lastSetPlusState, SET_PLUS_PIN, true)) {
			val = val >= 23 ? 0 : val + 1;
		}

		// user pressed the - button
		else if (wasButtonPushed(setMinusState, lastSetMinusState, SET_MINUS_PIN, true)) {
			val = val > 0 ? val - 1 : 23;
		}
	}

	return val;
}

/******************************************************************************/

void AlarmClock::printTime(int hr, int min, int sec, int mnth, int dy, int yr) {

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

	switch (curState) {

		case SET_SECOND:

			val = promptSecond();

			if (SET_TIME == mode) {
				setTime(hour(), minute(), val, month(), day(), year());
			}

			break;

		case SET_MINUTE:

			val = promptMinute();

			if (SET_TIME == mode) {
				setTime(hour(), val, second(), month(), day(), year());
			}

			break;

		case SET_HOUR:

			val = promptHour();

			if (SET_TIME == mode) {
				setTime(val, minute(), second(), month(), day(), year());
			}

			break;

		default:
			break;
	}

	if (SET_HOUR == curState) {
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

