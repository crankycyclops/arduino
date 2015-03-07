#include "include/utility.h"


bool wasButtonPushed(int &curState, int &lastState, int pin, bool analog) {

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

bool isLeapYear(int year) {

	return (year % 4 != 0) || (year % 400 == 0 && year % 100 != 0) ? false : true;
}

