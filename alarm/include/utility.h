#ifndef UTILITY_H
#define UTILITY_H

#include <Arduino.h>


// Returns true if the specified button was pushed and false otherwise. curState
// and lastState are references to variables used to keep track of a button's
// state, and analog determines whethe the button is connected to a digital or
// an analog input.
extern bool wasButtonPushed(int &curState, int &lastState, int pin, bool analog = false);

// Returns true if the given year is a leap year.
extern bool isLeapYear(int year);


#endif

