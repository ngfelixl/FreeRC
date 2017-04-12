#ifndef DS4_H
#define DS4_H
#endif

#include <PS4USB.h>
#include <usbhub.h>



class Ds4 {
typedef struct button {
	bool x, circle, square, triangle, up, down, left, right, options;
};

private:
	USB usb;
	PS4USB ps4 = PS4USB(&usb);

	uint8_t middle = 127;
	uint8_t deadzone = 50;
	void usb_setup();

public:
	char *status;
	uint8_t axis[6];
	button button;
	uint8_t battery;
	uint8_t motor;

	Ds4();
	void init();
	void get();
	bool connected();
};