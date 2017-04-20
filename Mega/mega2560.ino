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

// Declare and initiate radio data
uint64_t writingPipe = 0xF0F0F0F0AA;
uint64_t readingPipe = 0xF0F0F0F0BB;
RF24 radio(CE_pin, CSN_pin);
unsigned long radioTransmission = 0, readUsb = 0, counter_check_status = 0;


uint16_t transmission_success = 0;
uint16_t transmission_count = 0;
uint8_t transmission_quality = 0;

char* navigation_status = "";
uint8_t voltage = 0, voltage_count = 0;
float voltage_sum = 0, voltage_avg = 0;

uint8_t channel_map[4] = { 0, 0, 0, 0 }; // Values in range 0-7
uint8_t axis_range_min[4] = { 20, 20, 20, 20 }; // Values in range 0-7
uint8_t axis_range_max[4] = { 160, 160, 160, 160 }; // Values in range 0-7
uint8_t transmission[5] = { 0, 0, 0, 0, 0 }; // Moter, Channel 1-4

Ds4 controller;
Screen screen(&radio, channel_map, axis_range_min, axis_range_max);

void setup() {
	screen.init();
	controller.init();
	setupRadio(); // Setup NRF24L01+
}


void loop() {
	if (millis() - readUsb > 20) {
		readUsb = millis();
		controller.get();
		if (screen.view == "control") {
			for (uint8_t i = 0; i < 4; i++) {
				uint8_t axis_id = channel_map[i] / 2;
				if(axis_id >= 0 && axis_id <= 3)
					transmission[i + 1] = map(controller.axis[axis_id] * ((channel_map[i]+1)%2 - channel_map[i]%2) + channel_map[i]%2 * 255, 0, 255, axis_range_min[i], axis_range_max[i]);
			}
			screen.update_analog_axis(0, controller.axis[0], controller.axis[1], false);
			screen.update_analog_axis(1, controller.axis[2], controller.axis[3], false);

			screen.update_motor(controller.axis[4]);
			if (controller.button.options) {
				controller.status = "options";
				screen.switch_view("options");
			}
		}
		else if (screen.view == "options" || screen.view == "channels" || screen.view == "range") {
			navigation_status = screen.navigate(controller.button.left, controller.button.right, controller.button.up, controller.button.down, controller.button.x, controller.button.circle, controller.button.options);
		}
	}


	if (millis() - radioTransmission > 4) {
		// Write the radioData struct to the NRF24L01 module to send the data
		// and print success or error message
		radioTransmission = millis();
		uint8_t axis = map(controller.axis[0], 0, 255, 20, 160);

		radio.writeFast(&transmission, sizeof(transmission));

		if (radio.isAckPayloadAvailable()) {
			radio.read(&voltage, sizeof(voltage));
			voltage_sum = voltage_sum + voltage/10.0;
			voltage_count++;

			if (voltage_count == 20) {
				voltage_avg = voltage_sum / 20.0;
				voltage_sum = 0.0;
				voltage_count = 0;
			}
		}

		if (radio.txStandBy()) {
			transmission_success++;
		}


		//radio.setChannel(2);
		//radio.setChannel(3);
		//radio.setChannel(4);


		transmission_count++;
		if (transmission_count >= 100 || transmission_success >= 100) {
			transmission_quality = 10 * transmission_success / 100.0;
			transmission_count = 0;
			transmission_success = 0;
		}
	}


	if (millis() - counter_check_status > 500 && screen.view == "control") {
		// Check if a force update is required. This is the case when
		// returning to control view

		// Draw the plane object with the acceleration values (acceleration not implemented yet)
		// screen.draw_plane((controller.axis[0] - 127) /32.0, (controller.axis[1] - 127) / 32.0, -9);

		bool force = false;
		if (navigation_status == "back to control")
			force = true;

		// Update the battery level and the controller status
		screen.update_battery(controller.battery, force);
		screen.update_voltage(voltage_avg, force);
		if (controller.connected())
			screen.print_peripheral_status(2, "success", "Connected", force);
		else
			screen.print_peripheral_status(2, "danger", "Error", force);
		
		// Update the transmission quality
		if (transmission_quality > 8)
			screen.print_peripheral_status(0, "success", "Very good", force);
		else if (transmission_quality > 5)
			screen.print_peripheral_status(0, "warning", "Ok", force);
		else if (transmission_quality > 0)
			screen.print_peripheral_status(0, "danger", "Bad", force);
		else
			screen.print_peripheral_status(0, "danger", "No Signal", force);


		

		// Reset Force message
		if (navigation_status == "back to control")
			navigation_status = "";

		counter_check_status = millis();
	}
}

void setupRadio() {
	radio.begin();

	// Use PALevel low for testing purposes only (default: high)
	// PALevel now adjustable via options menu, default: high
	radio.setPALevel(RF24_PA_HIGH);
	radio.setDataRate(RF24_250KBPS);
	radio.setPayloadSize(sizeof(transmission));
	radio.setChannel(80);

	// Open a writing and reading pipe on each radio, with opposite addresses
	radio.openWritingPipe(writingPipe);
	radio.enableAckPayload();
	//radio.enableDynamicAck();
	//radio.openReadingPipe(1, readingPipe);
}
