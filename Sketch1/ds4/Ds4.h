#ifndef DS4_H
#define DS4_H
#endif

#include <PS4USB.h>
#include <usbhub.h>

class Ds4 {
private:
	USB usb;
	PS4USB ps4 = PS4USB(&usb);

	uint8_t middle = 127;
	uint8_t deadzone = 50;
	void usb_setup();
	//bool x, circle, square, triangle;
	//uint8_t axis[6];

public:
	uint8_t axis[6];
	struct button {
		bool x, circle, square, triangle;
	}button;

	Ds4();
	void init();
	void get();
	bool status();
};