#include"Ds4.h"

Ds4::Ds4() {
	PS4USB PS4(&Usb);
}

void Ds4::init() {
	//Usb = new USB();
	usb_setup();
}

void Ds4::get() {
	Usb.Task();
	// Check if DS4 is correctly recognized and get DS4 Data
	if (PS4->connected()) {
		axis[0] = PS4.getAnalogHat(LeftHatX);
		axis[1] = PS4.getAnalogHat(LeftHatY);
		axis[2] = PS4.getAnalogHat(RightHatX);
		axis[3] = PS4.getAnalogHat(RightHatY);
		axis[4] = PS4.getAnalogButton(L2);
		axis[5] = PS4.getAnalogButton(R2);
	}
}

void Ds4::usb_setup() {
	if (Usb.Init() == -1) {
		//peripheralData.statusId[1] = 2;
	}
	else {
		//peripheralData.statusId[1] = 1;
	}
}

bool Ds4::status() {
	Usb.Task();
	if (PS4.connected())
		return true;
	return false;
}