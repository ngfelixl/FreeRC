#include"Ds4.h"

Ds4::Ds4() {}

void Ds4::init() {
	USB usb;
	PS4USB ps4 = PS4USB(&usb);
	usb_setup();
}

void Ds4::get() {
	usb.Task();
	// Check if DS4 is correctly recognized and get DS4 Data
	
	if (ps4.connected()) {
		axis[0] = ps4.getAnalogHat(LeftHatX);
		axis[1] = ps4.getAnalogHat(LeftHatY);
		axis[2] = ps4.getAnalogHat(RightHatX);
		axis[3] = ps4.getAnalogHat(RightHatY);
		axis[4] = ps4.getAnalogButton(L2);
		axis[5] = ps4.getAnalogButton(R2);

		button.options = ps4.getButtonClick(OPTIONS);
		button.up = ps4.getButtonClick(UP);
		button.down = ps4.getButtonClick(DOWN);
		button.left = ps4.getButtonClick(LEFT);
		button.right = ps4.getButtonClick(RIGHT);
		button.x = ps4.getButtonClick(X);
		button.circle = ps4.getButtonClick(CIRCLE);
		button.square = ps4.getButtonClick(SQUARE);
		button.triangle = ps4.getButtonClick(TRIANGLE);
		button.r1 = ps4.getButtonClick(R1);
		button.l1 = ps4.getButtonClick(L1);

		battery = ps4.getBatteryLevel();
	}
	else {
		usb_setup();
	}
}

void Ds4::usb_setup() {
	if (usb.Init() == -1) {
	}
	else {
	}
}

bool Ds4::connected() {
	bool flag = false;
	usb.Task();
	if (ps4.connected())
		flag = true;
	return flag;
}