/*
 Name:		Sketch1.ino
 Created:	10.04.2017 12:00:00
 Author:	Felix Lemke

*  NRF24 Wiring (Arduino AtMeta 2560)
*  CE 40, CSN 53, MOSI - 50, MISO - 51
*  SCK 52, GND GND, VCC 3.3V
*
*  USB Host Shield plugged in
*/

#include <SPI.h>
#include "RF24.h"
#include "printf.h"
#include <PS4USB.h>
#include <usbhub.h>
#include "screen/Screen.h"
#include "ds4/Ds4.h"

#define CE_pin  53
#define CSN_pin 49

#define CHECK_STATUS 500

// Declare and initiate radio data
uint64_t writingPipe = 0xF0F0F0F0AA;
uint64_t readingPipe = 0xF0F0F0F0BB;
RF24 radio(CE_pin, CSN_pin);
unsigned long radioTransmission = 0, readUsb = 0, counter_check_status = 0;

float motor[2] = { 100,0 }; // new,old
int dmotor = 0;
struct radioData {
	uint8_t servo[3];
	uint8_t motor;
}radioData;

Screen screen(&radio);
Ds4 controller;


void setup() {
	Serial.begin(9600);
	screen.init();

	controller.init();

	// Setup NRF24L01+
	setupRadio();
}


void loop() {
	if (millis() - readUsb > 20) {
		controller.get();
		if (screen.view == "control") {
			screen.update_analog_axis(0, controller.axis[0]);
			screen.update_analog_axis(1, controller.axis[1]);
			screen.update_analog_axis(2, controller.axis[2]);
			screen.update_analog_axis(3, controller.axis[3]);
			screen.update_motor((float)controller.axis[4] / 255.0*100.0);
			if (controller.button.options) {
				screen.switch_view("options");
			}
		}
		else if (screen.view == "options") {
			screen.navigate(controller.button.left, controller.button.right, controller.button.up, controller.button.down, controller.button.x, controller.button.circle, controller.button.options);
		}
		readUsb = millis();
	}

	
	if (millis() - radioTransmission > 4) {
		// Write the radioData struct to the NRF24L01 module to send the data
		// and print success or error message

		uint8_t axis = (uint8_t)map((long)controller.axis[0], 0, 255, 20, 160);
		radio.setChannel(80);
		radio.writeFast(&axis, sizeof(axis));
		radio.txStandBy();
		radio.setChannel(2);
		radio.setChannel(3);
		radio.setChannel(4);

		radioTransmission = millis();
	}

	if (millis() - counter_check_status > CHECK_STATUS && screen.view == "control") {
		if (controller.connected()) {
			if (controller.status != "Connected") {
				controller.status = "Connected";
				screen.print_peripheral_status(2, "success", controller.status);
			}
		}
		else {
			if (controller.status != "Error") {
				controller.status = "Error";
				screen.print_peripheral_status(2, "danger", controller.status);
			}
		}

		counter_check_status = millis();
	}
	Serial.println(millis());
}

void setupRadio() {
	radio.begin();

	// Use PALevel low for testing purposes only (default: high)
	// PALevel now adjustable via options menu, default: high
	radio.setPALevel(RF24_PA_HIGH);
	radio.setPayloadSize(sizeof(uint8_t));

	// Open a writing and reading pipe on each radio, with opposite addresses
	radio.openWritingPipe(writingPipe);
	//radio.openReadingPipe(1, readingPipe);
}
