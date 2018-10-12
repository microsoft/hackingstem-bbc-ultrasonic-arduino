//===---------__ Hacking STEM – UltrasonicSensor.ino – Arduino __----------===//
// For use with the How deep is the ocean? Build an ultrasonic sensor to 
// measure ocean depths lesson planavailable from  Microsoft Education
// Workshop at http://aka.ms/hackingSTEM
//
// Overview:
// Each time one of the 5 switches is triggered, an ultrasonic reading is 
// sent on the serial connection.
//
//  Hardware Connections:
//  Arduino | HC-SR04 
//  -------------------
//    5V    |   VCC     
//    0     |   Switch 0     
//    1     |   Switch 1     
//    2     |   Switch 2     
//    3     |   Switch 3     
//    4     |   Switch 4     
//    9     |   Trig     
//    10    |   Echo     
//    13    |   LED     
//    GND   |   GND
//
//
// This project uses an Arduino UNO microcontroller board, information at:
// https://www.arduino.cc/en/main/arduinoBoardUno
//
// Comments, contributions, suggestions, bug reports, and feature requests
// are welcome! For source code and bug reports see:
// http://github.com/[TODO github path to Hacking STEM]
//
// Based on HC-SR04 project by SparkFun available:
// https://github.com/sparkfun/HC-SR04_UltrasonicSensor
//
// Copyright 2018 Jeremy Franklin-Ross, Microsoft EDU Workshop - HackingSTEM
// Copyright (c) 2016 SparkFun Electronic  (HC-SR04_UltrasonicSensor project)
// MIT License terms detailed in LICENSE.txt
//===----------------------------------------------------------------------===//



// LED Pin
const int LED_PIN = 13;

// Ultrasonic Pins
const int TRIG_PIN = 9;
const int ECHO_PIN = 10;

// Number of switch Pins
const int NUM_OF_SWITCH_PINS = 5;

// Anything over 400 cm (23200 us pulse) is "out of range"
const unsigned int MAX_DIST = 23200;

// Switches
int startPin = 2;
int readingPositionPin[NUM_OF_SWITCH_PINS];
int readPosition;
int lastReadPosition;

// led illumination vars
long ledTimer;
long currentTime = 0;
const int ledInterval = 100;

// defines variables
unsigned long t1;
unsigned long t2;
unsigned long pulse_width;
float cm;

void setup() {
	// Set up switch pins 
	for (int i = 0; i < NUM_OF_SWITCH_PINS; i++) {
		readingPositionPin[i] = startPin;
		pinMode(readingPositionPin[i], INPUT);
		digitalWrite(readingPositionPin[i], HIGH);
		startPin++;
	}

	// The set up led pin
	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, LOW);

	// The Trigger pin will tell the sensor to range find
	pinMode(TRIG_PIN, OUTPUT);
	digitalWrite(TRIG_PIN, LOW);

	// We'll use the serial monitor to view the sensor output
	Serial.begin(9600);
}

void loop() {
	// look for active pin
	for (int i = 0; i < NUM_OF_SWITCH_PINS; i++) {
		//int checkPosition = digitalRead (readingPositionPin[i]);
		if (digitalRead (readingPositionPin[i]) == 0) {
			readPosition = i + 1;
		}
	}

	// Hold the trigger pin high for at least 10 us
	digitalWrite(TRIG_PIN, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIG_PIN, LOW);

	// Wait for pulse on echo pin
	while( digitalRead(ECHO_PIN) == 0 );

	// Measure how long the echo pin was held high (pulse width)
	// Note: the micros() counter will overflow after ~70 min
	t1 = micros();
	while ( digitalRead(ECHO_PIN) == 1);
		t2 = micros();
	pulse_width = t2 - t1;

	// Calculate distance in centimeters and inches. The constants
	// are found in the datasheet, and calculated from the assumed speed 
	//of sound in air at sea level (~340 m/s).
	cm = pulse_width / 58.0;

	// Wait at least 60ms before next measurement
	delay(60);

	if (readPosition != lastReadPosition) {
		Serial.print(readPosition);
		Serial.print(",");
		Serial.print(cm);
		Serial.print("\n");
		lastReadPosition = readPosition;
		digitalWrite (LED_PIN, HIGH);
		ledTimer = millis();
	}
	currentTime = millis();
	if (currentTime - ledTimer >= ledInterval){
		digitalWrite (LED_PIN, LOW);
	}


}
