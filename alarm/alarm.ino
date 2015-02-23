#include <Arduino.h>
#include <Time.h>

#include "include/alarm.h"
#include "include/AlarmClock.h"


const char *monthNames[] = {
	"Jan",
	"Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"Jul",
	"Aug",
	"Sep",
	"Oct",
	"Nov",
	"Dec"
};

AlarmClock alarmClock;

/******************************************************************************/

void setup() {

	alarmClock.initLCD();
	alarmClock.initClock();
}

/******************************************************************************/

void loop() {

	// TODO: every so often, I should probably sync (read, not write) with the RTC
	alarmClock.updateDisplay();
}

