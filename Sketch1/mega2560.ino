/*
 Name:		Sketch1.ino
 Created:	13.02.2017 23:54:21
 Author:	felix 

*  NRF24 Wiring (Arduino AtMeta 2560)
*  CE   - 40
*  CSN  - 53
*  MOSI - 50
*  MISO - 51
*  SCK  - 52
*  GND  - GND
*  VCC  - 3.3V
*
*  USB Host Shield plugged in
*/

// Include required libraries
#include <SPI.h> // das ist eine spi bib
#include "RF24.h"
#include "printf.h"
#include <PS4USB.h>
#include <usbhub.h>
#include "screen/Screen.h"
#include "ds4/Ds4.h"

#define CE_pin  53
#define CSN_pin 49

#define CHECK_STATUS 1000

// Declare radio data
uint64_t writingPipe = 0xF0F0F0F0AA;
uint64_t readingPipe = 0xF0F0F0F0BB;
RF24 radio(CE_pin, CSN_pin);
unsigned int radioTransmission = 0, readUsb = 0, counter_check_status = 0;

float motor[2] = { 100,0 }; // new,old
int dmotor = 0;
struct radioData {
	uint8_t servo[3];
	uint8_t motor;
}radioData;

Screen screen;
Ds4 controller;


void setup() {
	Serial.begin(9600);

	screen.init();
	screen.print_peripheral_status(0, "success", "Hello world!");

	controller.init();

	// Setup NRF24L01+
	setupRadio();
}


void loop() {
	// x = x0 + dx / dt, 0.02s*100 = 2s
	if (millis() - readUsb > 20) {
		controller.get();
		if (screen.view == "control") {
			//Serial.println(controller.button.x);
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
			if (controller.button.up) {
				screen.options_navigate("up");
			}
			else if (controller.button.down) {
				screen.options_navigate("down");
			}
			if (controller.button.options) {
				screen.switch_view("control");
			}
		}
		//screen.print_peripheral_status(2, "success", "Connected");
		readUsb = millis();
	}

	
	if (millis() - radioTransmission > 20) {
		// Write the radioData struct to the NRF24L01 module to send the data
		// and print success or error message
		radio.stopListening();
		radio.writeFast(&radioData, sizeof(radioData));
		if (radio.txStandBy()) {
			//peripheralData.statusId[0] = 1; // success
		}
		else {
			//peripheralData.statusId[0] = 2; // failed
		}
		radioTransmission = millis();
	}

	if (millis() - counter_check_status > CHECK_STATUS) {
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
}


// ========== Screen related functions ==============
/*void print_to_view() {
	if(screen.update()) {
		//ds4feedback();
		motorFeedback();
	}
}*/

/*void motorFeedback() {
	// POS 10,100 DIM 25x130
	if (motor[0] != motor[1]) {
		//tft.fillRect(11, 101, 23, 128 - motor[0] / 100 * 128, BLACK);
		//tft.fillRect(11, 101 + 128 - motor[0] / 100 * 128, 23, motor[0] / 100 * 128, RED);
		motor[1] = motor[0];
	}
}*/

// Show DS4 Values on screen
/*void ds4read() {
	ds4val[0] = PS4.getAnalogHat(RightHatX);
	ds4val[1] = PS4.getAnalogHat(RightHatY);
	ds4val[2] = PS4.getAnalogHat(LeftHatX);
	// Apply deadzones
	if (abs(ds4val[0] - ds4mid)<ds4deadzone)
		ds4val[0] = ds4mid;
	if (abs(ds4val[1] - ds4mid)<ds4deadzone)
		ds4val[1] = ds4mid;
	if (abs(ds4val[2] - ds4mid)<ds4deadzone)
		ds4val[2] = ds4mid;

	// Control motor with L2/R2
	if (PS4.getAnalogButton(L2) > 50 && PS4.getAnalogButton(R2) <= 50) {
		dmotor = -(PS4.getAnalogButton(L2)-50.0) / 205.0 * 3.0;
	}
	else if (PS4.getAnalogButton(R2) > 50 && PS4.getAnalogButton(L2) <= 50) {
		dmotor = (PS4.getAnalogButton(R2)-50.0) / 205.0 * 3.0;
	}
	else {
		dmotor = 0;
	}

	motor[0] = motor[0] + (float)dmotor;
	if (motor[0] > 100.0) motor[0] = 100.0;
	else if (motor[0] < 0.0) motor[0] = 0.0;
	radioData.motor = (int)map(motor[0], 0, 100, 20, 160);
}*/

void setupRadio() {
	// Serial.print("radio");
	radio.begin();

	// Use PALevel low for testing purposes only (default: high)
	radio.setPALevel(RF24_PA_HIGH);
	radio.setPayloadSize(sizeof(radioData));

	// Open a writing and reading pipe on each radio, with opposite addresses
	radio.openWritingPipe(writingPipe);
	radio.openReadingPipe(1, readingPipe);
}
