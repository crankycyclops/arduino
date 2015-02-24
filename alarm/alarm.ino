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
	alarmClock.readRTC();
}

/******************************************************************************/

void loop() {

	// sync with the RTC every four hours
	if (0 == millis() % (4 * MILLIS_IN_HOUR)) {
		alarmClock.readRTC();
	}

	alarmClock.updateDisplay();
}

