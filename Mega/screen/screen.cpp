#include"Screen.h"


Screen::Screen(RF24 *radio) {
	tft = new Adafruit_TFTLCD(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
	this->radio = radio;
	menu = Menu(tft, radio);
}

void Screen::init() {
	tft->reset();
	tft->begin(0x9341);
	tft->setRotation(3);
	initial_view();
	print_servo_default();
}

bool Screen::update() {
	if (millis() - update_counter > SCREENUPDATE) {
		update_counter = millis();
		return true;
	}
	return false;
}

char* Screen::navigate(bool left, bool right, bool up, bool down, bool x, bool circle, bool options) {
	char *action = menu.execute(left, right, x, circle);
	char *output = "";

	if (action == "exit") {
		switch_view("control");
	}

	if (up) {
		menu.previous();
	}
	else if (down) {
		menu.next();
	}
	if (options || circle) {
		switch_view("control");
		output = "back to control";
	}
	return output;
}

void Screen::print_peripheral_status(int id, char *type, char *message, bool force) {
	char *status = "";
	bool status_updated=false; 

	if (id == 0)
		status = transmitter_status;
	else if (id == 2)
		status = controller_status;


	if (status != message || force) {
		int color = WHITE;
		if (type == "success") color = GREEN;
		else if (type == "warning") color = YELLOW;
		else if (type == "danger") color = RED;

		tft->setTextColor(color);
		tft->setCursor(150, 40 + 10 * id);
		tft->fillRect(150, 40 + 10 * id, 100, 10, BLACK);
		tft->println(message);
		status_updated = true;
	}

	if (status_updated) {
		if (id == 0)
			transmitter_status = message;
		else if (id == 2)
			controller_status = message;
	}
}

void Screen::print_servo_default() {
	// Servo field dim 175x130, pos 67,100
	tft->drawLine(67 + 175 / 2, 120, 67 + 175 / 2, 210, GREEN);
	tft->drawLine(67 + 20, 100 + 130 / 2, 67 + 175 - 20, 100 + 130 / 2, GREEN);
}

void Screen::update_analog_axis(uint8_t axis, uint8_t x, uint8_t y, bool force) {

	uint8_t width = 48;
	uint8_t height = 48;

	if (axis == 0) { // Left Stick
		if (x != left_axis_pos[0] || y != left_axis_pos[1] || force) {
			tft->drawLine(11 + left_axis_pos[0] / 255.0*width, 177 + left_axis_pos[1] / 255.0*height, 15 + left_axis_pos[0] / 255.0*width, 177 + left_axis_pos[1] / 255.0 * height, BLACK);
			tft->drawLine(13 + left_axis_pos[0] / 255.0*width, 175 + left_axis_pos[1] / 255.0*height, 13 + left_axis_pos[0] / 255.0*width, 179 + left_axis_pos[1] / 255.0 * height, BLACK);

			tft->drawLine(11 + x / 255.0*width, 177 + y / 255.0*height, 15 + x / 255.0*width, 177 + y / 255.0 * height, ORANGE);
			tft->drawLine(13 + x / 255.0*width, 175 + y / 255.0*height, 13 + x / 255.0*width, 179 + y / 255.0 * height, ORANGE);

			left_axis_pos[0] = x;
			left_axis_pos[1] = y;
		}
	}else if (axis == 1) { // Left Stick
		if (x != right_axis_pos[0] || y != right_axis_pos[1] || force) {
			tft->drawLine(255 + right_axis_pos[0] / 255.0*width, 177 + right_axis_pos[1] / 255.0*height, 259 + right_axis_pos[0] / 255.0*width, 177 + right_axis_pos[1] / 255.0 * height, BLACK);
			tft->drawLine(257 + right_axis_pos[0] / 255.0*width, 175 + right_axis_pos[1] / 255.0*height, 257 + right_axis_pos[0] / 255.0*width, 179 + right_axis_pos[1] / 255.0 * height, BLACK);

			tft->drawLine(255 + x / 255.0*width, 177 + y / 255.0*height, 259 + x / 255.0*width, 177 + y / 255.0 * height, ORANGE);
			tft->drawLine(257 + x / 255.0*width, 175 + y / 255.0*height, 257 + x / 255.0*width, 179 + y / 255.0 * height, ORANGE);

			right_axis_pos[0] = x;
			right_axis_pos[1] = y;
		}
	}
}

void Screen::update_motor(uint8_t motor) {
	//if (motor != motor_state) {
		tft->fillRect(11, 101, 4, 62 - motor / 255.0 * 62, DARKGRAY);
		tft->fillRect(11, 101 + 62 - motor / 255.0 * 62, 4, motor / 255.0 * 62, ORANGE);
		motor_state = motor;
	//}
}

void Screen::update_battery(uint8_t ds4_battery, bool force) {
	if (ds4_battery_state != ds4_battery || force) {
		tft->fillRect(303, 11, 6, 18-ds4_battery/15.0*18, DARKGRAY);
		tft->fillRect(303, 11+18-18*ds4_battery/15.0, 6, ds4_battery/15.0*18, GREEN);
		ds4_battery_state = ds4_battery;
	}
}

void Screen::switch_view(String change_to) {
	view = change_to;
	if (view == "control") initial_view();
	else if (view == "options") menu.display("");
}


void Screen::initial_view() {
	tft->fillScreen(BLACK);
	tft->setCursor(10, 10);
	tft->setTextColor(ORANGE);
	tft->setTextSize(2);
	tft->println("RC Flight Control");
	tft->setTextColor(WHITE);
	tft->setTextSize(1);

	// Add peripherals entry under title
	tft->setCursor(10, 40);
	tft->println("NRF24L01+");
	tft->setCursor(10, 50);
	tft->println("USB Host Shield");
	tft->setCursor(10, 60);
	tft->println("PS4 Controller");

	// Add subsection titles
	tft->setTextColor(ORANGE);
	tft->setCursor(10, 75);
	tft->println("Device Feedback");
	tft->drawLine(10, 85, 242, 85, ORANGE);
	tft->setCursor(260, 75);
	tft->println("DS4");
	tft->drawLine(260, 85, 310, 85, ORANGE);
	tft->setTextColor(WHITE);

	// Draw Motor Status box
	tft->setCursor(10, 90);
	tft->println("Motor");
	tft->drawRect(10, 100, 6, 64, WHITE);
	tft->setTextColor(TEALBLUE);
	/*tft->setCursor(40, 95);
	tft->println("100");
	tft->setCursor(40, 160);
	tft->println("50");
	tft->setCursor(40, 225);
	tft->println("0");
	tft->setTextColor(WHITE);
	*/
	// Draw Left Axis Box
	tft->drawRect(10, 174, 56, 56, WHITE);

	// Draw Right Axis Box
	tft->drawRect(254, 174, 56, 56, WHITE);

	// Draw Battery Level Boxes
	tft->drawRect(302, 10, 8, 20, WHITE);

	// Draw Accelerometer box
	tft->setCursor(67, 90);
	tft->println("Servo");
	tft->drawRect(76, 100, 168, 130, WHITE);

	// Draw PS4 Status box
	/*for (int i = 0; i<2; i++) {
		for (int j = 0; j<2; j++) {
			tft->drawRect(260 + i * 30, 100 + j * 75, 20, 55, WHITE);
			tft->drawLine(256 + i * 30, 101 + 26 + j * 75, 259 + i * 30, 101 + 26 + j * 75, WHITE);
		}
	}*/
}
