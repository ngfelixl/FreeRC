#ifndef DS4_H
#define DS4_H
#endif

#include <PS4USB.h>
#include <usbhub.h>

class Ds4 {
private:
	USB Usb;
	PS4USB PS4(USB);

	uint8_t middle = 127;
	uint8_t deadzone = 50;
	bool x, circle, square, triangle;
	//uint8_t axis[6];
	void usb_setup();

public:
	uint8_t axis[6];

	Ds4();
	void init();
	void get();
	bool status();
};