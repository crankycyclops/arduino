#include "include/utility.h"

#define ANALOG_BUTTON_THRESHOLD 100


bool wasButtonPushed(int &curState, int &lastState, int pin, bool analog) {

	bool pushed = false;

	// With 10K pull-down resistors and a USB power supply, analog pins will
	// read 0 when the button isn't pressed. But when using a 12V power supply
	// and the same pull-down resistors, the pin will still read non-zero. So
	// I've experimentally determined a threshold that may or may not work in
	// all circumstances.
	if (analog) {
		curState = analogRead(pin) > ANALOG_BUTTON_THRESHOLD ? HIGH : LOW;
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

bool isLeapYear(int year) {

	return (year % 4 != 0) || (year % 400 == 0 && year % 100 != 0) ? false : true;
}

