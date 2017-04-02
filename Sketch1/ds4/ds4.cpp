#include"Ds4.h"

Ds4::Ds4() {}

void Ds4::init() {
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

		button.x = ps4.getButtonClick(X);
	}
}

void Ds4::usb_setup() {
	if (usb.Init() == -1) {
		//peripheralData.statusId[1] = 2;
	}
	else {
		//peripheralData.statusId[1] = 1;
	}
}

bool Ds4::status() {
	usb.Task();
	if (ps4.connected())
		return true;
	return false;
}