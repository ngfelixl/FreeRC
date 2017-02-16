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
#include <SPI.h>
#include "RF24.h"
#include "printf.h"
#include <PS4USB.h>
#include <usbhub.h>
#include <SPFD5408_Adafruit_GFX.h>
#include <SPFD5408_Adafruit_TFTLCD.h>

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4
#define SCREENUPDATE 100 // Update screen every x[ms]

#define CE_pin 53
#define CSN_pin 49

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// Declare radio data
//byte addresses[][6] = { "1Node","2Node" };
uint64_t writingPipe = 0xF0F0F0F0AA;
uint64_t readingPipe = 0xF0F0F0F0BB;
//bool radioNumber = 0; // radio ID;
RF24 radio(CE_pin, CSN_pin);
unsigned int radioTransmission = 0, readUsb = 0;

float motor[2] = { 100,0 }; // new,old
int dmotor = 0;
struct radioData {
	unsigned int servo[3];
	unsigned int motor;
}radioData;

// Declare DualShock 4 Data
USB Usb;
PS4USB PS4(&Usb);
int ds4mid = 127;
int ds4deadzone = 50;
int ds4val[8]; // rx, ry, lx, ly

			   // Declare TFT Data
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
unsigned long lastScreenUpdate = 0;

struct peripheralData {
	unsigned int statusId[3] = { 0,0,0 }; // [radio, usb, ps4]
	unsigned int lastStatusId[3] = { 1,1,1 };
	String messages[3] = { "Initialize", "Connected", "No connection"};
}peripheralData;


// ========== Arduino Setup Function ==============
void setup() {
	Serial.begin(9600);
	// Reset and initialize TFT Screen (SDFP5408)
	tft.reset();
	tft.begin(0x9341);
	tft.setRotation(3);
	initScreen();
	updateStatusMessages(0);
	updateStatusMessages(1);
	updateStatusMessages(2);

	// Setup NRF24L01+ and USB Host Shield
	setupUsb();
	setupRadio();
}


// ========== Arduino Loop Function ==============
void loop() {
	// x = x0 + dx / dt, 0.02s*100 = 2s
	if (millis() - readUsb > 20) {
		// Activate USB
		Usb.Task();
		// Check if DS4 is correctly recognized and get DS4 Data
		if (PS4.connected()) {
			peripheralData.statusId[2] = 1; // update status
			// Read DS4 Data
			ds4read();

			// Store DS4 Data in structure to transmit to Nano
			radioData.servo[0] = map(ds4val[0], 0, 255, 20, 160);
			radioData.servo[1] = map(ds4val[1], 0, 255, 20, 160);
			radioData.servo[2] = map(ds4val[2], 0, 255, 20, 160);
		}
		else {
			// Print: DS4 Error
			peripheralData.statusId[2] = 2;
		}
		readUsb = millis();
	}

	// Write the radioData struct to the NRF24L01 module to send the data
	// and print success or error message
	if (millis() - radioTransmission > 20) {
		radio.stopListening();
		radio.writeFast(&radioData, sizeof(radioData));
		if (radio.txStandBy()) {
			peripheralData.statusId[0] = 1; // success
		}
		else {
			peripheralData.statusId[0] = 2; // failed
		}
		radioTransmission = millis();
	}

	// Update Screen Information
	screenUpdate();
}


// ========== Screen related functions ==============
void screenUpdate() {
	if (millis() - lastScreenUpdate>SCREENUPDATE) {
		updateStatusMessages(0);
		updateStatusMessages(1);
		updateStatusMessages(2);
		ds4feedback();
		motorFeedback();
		lastScreenUpdate = millis();
	}
}

void motorFeedback() {
	// POS 10,100 DIM 25x130
	if (motor[0] != motor[1]) {
		tft.fillRect(11, 101, 23, 128 - motor[0] / 100 * 128, BLACK);
		tft.fillRect(11, 101 + 128 - motor[0] / 100 * 128, 23, motor[0] / 100 * 128, RED);
		motor[1] = motor[0];
	}
}

// Show DS4 Values on screen
void ds4read() {
	ds4val[0] = PS4.getAnalogHat(RightHatX);
	ds4val[1] = PS4.getAnalogHat(RightHatY);
	ds4val[2] = PS4.getAnalogHat(LeftHatX);
	//ds4val[3] = PS4.getAnalogHat(LeftHatY);
	Serial.println(PS4.getAnalogButton(L2));
	// Apply deadzones
	if (abs(ds4val[0] - ds4mid)<ds4deadzone)
		ds4val[0] = ds4mid;
	if (abs(ds4val[1] - ds4mid)<ds4deadzone)
		ds4val[1] = ds4mid;
	if (abs(ds4val[2] - ds4mid)<ds4deadzone)
		ds4val[2] = ds4mid;
	if (PS4.getAnalogButton(L2) > 50 && PS4.getAnalogButton(R2) <= 50) {
		dmotor = -(PS4.getAnalogButton(L2)-50.0) / 205.0 * 3.0;
	}
	else if (PS4.getAnalogButton(R2) > 50 && PS4.getAnalogButton(L2) <= 50) {
		dmotor = (PS4.getAnalogButton(R2)-50.0) / 205.0 * 3.0;
	}
	else {
		dmotor = 0;
	}
	/*if (ds4val[3]>ds4mid + ds4deadzone && motor[0]>0) {
		dmotor = -1;
	}
	else if (ds4val[3]<ds4mid - ds4deadzone ) {
		dmotor = +1;
	}
	else {
		dmotor = 0;
	}*/

	motor[0] = motor[0] + (float)dmotor;
	if (motor[0] > 100.0) motor[0] = 100.0;
	else if (motor[0] < 0.0) motor[0] = 0.0;
	radioData.motor = (int)map(motor[0], 0, 100, 20, 160);
}

void ds4feedback() {
	for (int i = 0; i<2; i++) {
		for (int j = 0; j<2; j++) {
			if (ds4val[i + 2 * j] != ds4val[i + 2 * j + 4]) {
				tft.fillRect(261 + i * 30, 101 + j * 75 + (53 - ((float)ds4val[i + 2 * j] / 255) * 53), 18, ((float)ds4val[i + 2 * j] / 255) * 53, BLUE);
				tft.fillRect(261 + i * 30, 101 + j * 75, 18, 53 - ((float)ds4val[i + 2 * j] / 255) * 53, BLACK);
				ds4val[i + 2 * j + 4] = ds4val[i + 2 * j];
			}
		}
	}
}

void updateStatusMessages(int id) {
	switch (peripheralData.statusId[id]) {
	case 0: tft.setTextColor(YELLOW); break;
	case 1: tft.setTextColor(GREEN); break;
	case 2: tft.setTextColor(RED); break;
	}

	if (peripheralData.lastStatusId[id] != peripheralData.statusId[id]) {
		tft.fillRect(150, 40 + 10 * id, 320, 10, BLACK);
		tft.setCursor(150, 40 + 10 * id);
		if (peripheralData.statusId[id]<2) {
			tft.println(peripheralData.messages[peripheralData.statusId[id]]);
		}
		else {
			tft.println(peripheralData.messages[2]);
		}
		peripheralData.lastStatusId[id] = peripheralData.statusId[id];
	}
	tft.setTextColor(WHITE);
}

void initScreen() {
	tft.fillScreen(BLACK);
	tft.setCursor(10, 10);
	tft.setTextColor(RED);
	tft.setTextSize(2);
	tft.println("RC Flight Control");
	tft.setTextColor(WHITE);
	tft.setTextSize(1);

	// Add peripherals entry under title
	tft.setCursor(10, 40);
	tft.println("NRF24L01+");
	tft.setCursor(10, 50);
	tft.println("USB Host Shield");
	tft.setCursor(10, 60);
	tft.println("PS4 Controller");

	// Add subsection titles
	tft.setTextColor(MAGENTA);
	tft.setCursor(10, 75);
	tft.println("Nano Feedback");
	tft.drawLine(10, 85, 242, 85, MAGENTA);
	tft.setCursor(260, 75);
	tft.println("DS4");
	tft.drawLine(260, 85, 310, 85, MAGENTA);
	tft.setTextColor(WHITE);

	// Draw Motor Status box
	tft.setCursor(10, 90);
	tft.println("Motor");
	tft.drawRect(10, 100, 25, 130, WHITE);
	tft.setTextColor(CYAN);
	tft.setCursor(40, 95);
	tft.println("100");
	tft.setCursor(40, 160);
	tft.println("50");
	tft.setCursor(40, 225);
	tft.println("0");
	tft.setTextColor(WHITE);

	// Draw Global Servo Status box
	tft.setCursor(67, 90);
	tft.println("Servo");
	tft.drawRect(67, 100, 175, 130, WHITE);
	drawServoDefaults();

	// Draw PS4 Status box
	for (int i = 0; i<2; i++) {
		for (int j = 0; j<2; j++) {
			tft.drawRect(260 + i * 30, 100 + j * 75, 20, 55, WHITE);
			tft.drawLine(256 + i * 30, 101 + 26 + j * 75, 259 + i * 30, 101 + 26 + j * 75, WHITE);
			tft.drawLine(256 + i * 30, 101 + 53 / 2 + ((float)ds4deadzone / 255) * 53 + j * 75, 259 + i * 30, 101 + 53 / 2 + ((float)ds4deadzone / 255) * 53 + j * 75, RED);
			tft.drawLine(256 + i * 30, 101 + 53 / 2 - ((float)ds4deadzone / 255) * 53 + j * 75, 259 + i * 30, 101 + 53 / 2 - ((float)ds4deadzone / 255) * 53 + j * 75, RED);
		}
	}
}

void drawServoDefaults() {
	// Servo field dim 175x130, pos 67,100
	tft.drawLine(67 + 175 / 2, 120, 67 + 175 / 2, 210, GREEN);
	tft.drawLine(67 + 20, 100 + 130 / 2, 67 + 175 - 20, 100 + 130 / 2, GREEN);
}

// ========== Periphal Setup functions ==============
void setupUsb() {
	if (Usb.Init() == -1) {
		peripheralData.statusId[1] = 2;
	} else {
		peripheralData.statusId[1] = 1;
	}
}

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
