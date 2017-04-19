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
#include <stdlib.h>
#include <stdio.h>

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
unsigned long radioRead = 0, servoWrite = 0, readBattery = 0;

uint8_t transmission[5];
uint8_t voltage;

void setup() {
	Serial.begin(9600);
	radio.begin();
	delay(100);

	// Set the PA Level low to prevent power supply related issues since this is a
	// getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
	radio.setPALevel(RF24_PA_LOW);
	radio.setPayloadSize(sizeof(transmission));
	radio.enableAckPayload();

	// Open a writing and reading pipe on each radio, with opposite addresses
	radio.openReadingPipe(1, readingPipe);
	radio.openWritingPipe(writingPipe);

	servo[0].attach(MOTOR_PIN);
	servo[1].attach(SERVO_LEFT_PIN);
	servo[2].attach(SERVO_RIGHT_PIN);
	servo[3].attach(SERVO_HEIGHT_PIN);
	servo[4].attach(SERVO_SIDE_PIN);
}

void loop() {
	radio.startListening();
	radio.setChannel(80);
	if (radio.available()) {
		radio.read(&transmission, sizeof(transmission));
		radio.writeAckPayload(1, &voltage, sizeof(voltage));
	}

	if (millis() - readBattery > 500) {
		voltage = analogRead(0) / 1024.0 * 5 * 2.5 / 1.1 * 10;
		Serial.println(voltage);
		readBattery = millis();
	}

	if (millis() - servoWrite > 5) {
		//Serial.println(radioData.motor);
		// MOTOR SERVO
		//setServo(0, radioData.motor);
		//setServo(1, radioData.servo[0]);
		//setServo(2, 180-radioData.servo[0]);
		//setServo(3, radioData.servo[1]);
		//setServo(4, radioData.servo[2]);
		for (uint8_t i = 0; i < 5; i++) {
			setServo(i, transmission[i]);
		}

		//setServo(4, transmission[1]);
		servoWrite = millis();

	}
}

void setServo(uint8_t id, uint8_t value) {
	if (value != NULL && value >= 0 && value <= 180) {
		servo[id].write(value);
	}
}
