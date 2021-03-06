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
#define MOTOR 2
#define CHANNEL_1 3
#define CHANNEL_2 4
#define CHANNEL_3 5
#define CHANNEL_4 6

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
	radio.setDataRate(RF24_250KBPS);
	radio.setPayloadSize(sizeof(transmission));
	radio.enableAckPayload();

	// Open a writing and reading pipe on each radio, with opposite addresses
	radio.openReadingPipe(1, readingPipe);
	radio.openWritingPipe(writingPipe);

	servo[0].attach(MOTOR);
	servo[1].attach(CHANNEL_1);
	servo[2].attach(CHANNEL_2);
	servo[3].attach(CHANNEL_3);
	servo[4].attach(CHANNEL_4);
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
		for (uint8_t i = 0; i < 5; i++) {
			setServo(i, transmission[i]);
		}

		servoWrite = millis();
	}
}

void setServo(uint8_t id, uint8_t value) {
	if (value != NULL && value >= 0 && value <= 180) {
		servo[id].write(value);
	}
}
