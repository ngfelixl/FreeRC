/*
 Name:		Sketch2.ino
 Created:	14.02.2017 21:15:23
 Author:	felix

* CE   - 7
* CSN  - 8
* MOSI - 11
* MISO - 12
* SCK  - 13
* GND  - GND
* VCC  - 3.3V

* Servo Wiring:
* VCC  - 5V
* GND  - GND
* CTRL - 2
*/

#include <SPI.h>
#include "RF24.h"
#include "printf.h"
#include <Servo.h>

// Define Pins for NRF24
#define CE_PIN 7
#define CSN_PIN 8

// Define Pins for Servos
#define MOTOR_PIN 2
#define SERVO_LEFT_PIN 3
#define SERVO_RIGHT_PIN 4
#define SERVO_HEIGHT_PIN 5
#define SERVO_SIDE_PIN 6

Servo servo[5]; // 4=motor, 0=left, 1=right=-left, 2=height-ctr, 3=side-ctr

//byte addresses[][6] = { "1Node","2Node" };
uint64_t readingPipe = 0xF0F0F0F0AA;
//bool radioNumber = 1; // radio ID
RF24 radio(CE_PIN, CSN_PIN);
int radioRead=0, servoWrite;

float motor = 160;
unsigned int motor_config[3] = { 90, 30, 60 }; // {offset, threshold 1, threshold 2}
unsigned int motorCtrlPause = 500; // [ms]
unsigned int motorCtrlLast = 0;

struct {
	unsigned int servo[3]; // 0=roll, 1=height, 2=side
	unsigned int motor; // servo position values
}radioData;

void setup() {
	Serial.begin(9600);
	radio.begin();

	// Set the PA Level low to prevent power supply related issues since this is a
	// getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
	radio.setPALevel(RF24_PA_HIGH);
	radio.setPayloadSize(sizeof(radioData));

	// Open a writing and reading pipe on each radio, with opposite addresses
	radio.openReadingPipe(1, readingPipe);

	// Start the radio listening for data
	//radio.setPayloadSize(sizeof(radioData));
	radio.startListening();

	servo[4].attach(MOTOR_PIN);
	servo[0].attach(SERVO_LEFT_PIN);
	servo[1].attach(SERVO_RIGHT_PIN);
	servo[2].attach(SERVO_HEIGHT_PIN);
	servo[3].attach(SERVO_SIDE_PIN);
}

void loop() {
	//if (radio.available() && millis() - radioRead > 50) {
		//while(radio.available())
	if(radio.available())
		radio.read(&radioData, sizeof(radioData));
		//radioRead = millis();
	//}

	// As long as getting data -> read it


	//while (radio.available()) {
	//}
	//Serial.println(radioData.motor);

	if (millis() - servoWrite > 100) {
		Serial.println(radioData.motor);
		// MOTOR SERVO
		if (radioData.motor >= 0 && radioData.motor <= 180) {
			servo[4].write(radioData.motor);
		}

		// CONTROL SERVOS
		if (radioData.servo[0] >= 0 && radioData.servo[0] <= 180) {
			servo[0].write(radioData.servo[0]);
			servo[1].write(180 - radioData.servo[0]);
		}
		if (radioData.servo[1] >= 0 && radioData.servo[1] <= 180) {
			servo[2].write(radioData.servo[1]);
		}

		if (radioData.servo[2] >= 0 && radioData.servo[2] <= 180) {
			servo[3].write(radioData.servo[2]);
		}
		servoWrite = millis();
	}
}
