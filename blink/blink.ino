#include <Arduino.h>

// A variation on the classic blink program. This version creates a simple
// rolling pattern of lights.
const int LED_1 = 2;
const int LED_2 = 7;
const int LED_3 = 4;

/******************************************************************************/

void setup() {

	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(LED_1, OUTPUT);
	pinMode(LED_2, OUTPUT);
	pinMode(LED_3, OUTPUT);

	digitalWrite(LED_BUILTIN, LOW);
	digitalWrite(LED_1, LOW);
	digitalWrite(LED_2, LOW);
	digitalWrite(LED_3, LOW);
}

/******************************************************************************/

void loop() {

	digitalWrite(LED_3, LOW);
	digitalWrite(LED_BUILTIN, HIGH);
	delay(500);
	digitalWrite(LED_BUILTIN, LOW);
	digitalWrite(LED_1, HIGH);
	delay(500);
	digitalWrite(LED_1, LOW);
	digitalWrite(LED_2, HIGH);
	delay(500);
	digitalWrite(LED_2, LOW);
	digitalWrite(LED_3, HIGH);
	delay(500);
}

