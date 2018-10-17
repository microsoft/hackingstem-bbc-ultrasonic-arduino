//===---------__ Hacking STEM – UltrasonicSensor.ino – Arduino __----------===//
// For use with the How deep is the ocean? Build an ultrasonic sensor to 
// measure ocean depths lesson plan available from  Microsoft Education
// Workshop at http://aka.ms/hackingSTEM
//
// Overview:
// Each time one of the 5 switches is triggered, an ultrasonic reading is 
// sent on the serial connection.
//
//  Hardware Connections:
//     Pin     |    Connection
//  ------------------------------
//      9      |   Trig HC-SR04     
//      10     |   Echo HC-SR04     
//      GND    |   GND  HC-SR04 
//      5V     |   VCC HC-SR04     
//   0,1,2,3,4 | Switch 0,1,2,3,4     
//      13     |   LED     
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
// Copyright 2018 Adi Adzulay, Jeremy Franklin-Ross, 
// Microsoft EDU Workshop - HackingSTEM
// Copyright (c) 2016 SparkFun Electronic  (HC-SR04_UltrasonicSensor project)
// MIT License terms detailed in LICENSE.txt
//===----------------------------------------------------------------------===//

// LED Pin
const int kLedPin = 13;

// Ultrasonic Pins
const int kTrigPin = 9;
const int kEchoPin = 10;

// Number of switch Pins
const int kNumOfSwitchPins = 5;

// Anything over 400 cm (23200 us pulse) is "out of range"
const unsigned int kMaxDistance = 23200;

// Switches
int startPin = 2;
int readingPositionPin[kNumOfSwitchPins];
int readPosition;
int lastReadPosition;

// led illumination variables
long ledTimer;
long currentTime = 0;
const int ledInterval = 100;

// defines variables
unsigned long time1;
unsigned long time2;
unsigned long pulse_width;
float mm;

void setup() {
	// Set up switch pins 
	for (int i = 0; i < kNumOfSwitchPins; i++) {
		readingPositionPin[i] = startPin;
		pinMode(readingPositionPin[i], INPUT);
		digitalWrite(readingPositionPin[i], HIGH);
		startPin++;
	}

	// The set up led pin
	pinMode(kLedPin, OUTPUT);
	digitalWrite(kLedPin, LOW);

	// The Trigger pin will tell the sensor to range find
	pinMode(kTrigPin, OUTPUT);
	digitalWrite(kTrigPin, LOW);

	// We'll use the serial monitor to view the sensor output
	Serial.begin(9600);
}

void loop() {
	// look for active pin
	for (int i = 0; i < kNumOfSwitchPins; i++) {
		if (digitalRead (readingPositionPin[i]) == LOW) {
			readPosition = i + 1;
		}
	}

	// Hold the trigger pin high for at least 10 us
	digitalWrite(kTrigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(kTrigPin, LOW);

	// Wait for pulse on echo pin
	while( digitalRead(kEchoPin) == LOW );

	// Measure how long the echo pin was held high (pulse width)
	// Note: the micros() counter will overflow after ~70 min
	time1 = micros();
	while ( digitalRead(kEchoPin) == HIGH );
		time2 = micros();
	pulse_width = time2 - time1;

	// Calculate distance in centimeters and inches. The constants
	// are found in the datasheet, and calculated from the assumed speed 
	//of sound in air at sea level (~340 m/s).
	mm = pulse_width / 5.80;

	// Wait at least 60ms before next measurement
	delay(60);

	if (readPosition != lastReadPosition) {
		Serial.print(readPosition);
		Serial.print(",");
		Serial.print(int(mm));
		Serial.print("\n");
		lastReadPosition = readPosition;
		digitalWrite (kLedPin, HIGH);
		ledTimer = millis();
	}
	
	currentTime = millis();

	if (currentTime - ledTimer >= ledInterval){
		digitalWrite (kLedPin, LOW);
	}
}
