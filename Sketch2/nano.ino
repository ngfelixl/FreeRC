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

uint64_t readingPipe = 0xF0F0F0F0AA;
uint64_t writingPipe = 0xF0F0F0F0BB;
RF24 radio(CE_PIN, CSN_PIN);
unsigned int radioRead=0, servoWrite=0;

struct {
	uint8_t servo[3]; // 0=roll, 1=height, 2=side
	uint8_t motor; // servo position values
}radioData;

void setup() {
	Serial.begin(9600);
	radio.begin();

	// Set the PA Level low to prevent power supply related issues since this is a
	// getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
	radio.setPALevel(RF24_PA_HIGH);
	radio.setPayloadSize(sizeof(radioData));
	radio.enableAckPayload();

	// Open a writing and reading pipe on each radio, with opposite addresses
	radio.openReadingPipe(1, readingPipe);
	radio.openWritingPipe(writingPipe);

	servo[4].attach(MOTOR_PIN);
	servo[0].attach(SERVO_LEFT_PIN);
	servo[1].attach(SERVO_RIGHT_PIN);
	servo[2].attach(SERVO_HEIGHT_PIN);
	servo[3].attach(SERVO_SIDE_PIN);
}

void loop() {
	radio.startListening();
	if (radio.available()) {
		radio.read(&radioData, sizeof(radioData));
	}
	radio.writeAckPayload(1, &radioData.motor, sizeof(radioData.motor));

	if (millis() - servoWrite > 200) {
		Serial.println(radioData.motor);
		// MOTOR SERVO
		setServo(4, radioData.motor);
		setServo(0, radioData.servo[0]);
		setServo(1, 180-radioData.servo[0]);
		setServo(2, radioData.servo[1]);
		setServo(3, radioData.servo[2]);

		servoWrite = millis();
	}
}

void setServo(uint8_t id, uint8_t value) {
	if (value >= 0 && value <= 180) {
		servo[id].write(value);
	}
}
