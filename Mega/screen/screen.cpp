#include"Screen.h"


Screen::Screen(RF24 *radio, uint8_t *channel_map, uint8_t *axis_range_min, uint8_t *axis_range_max) {
	tft = new Adafruit_TFTLCD(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
	this->radio = radio;
	this->channel_map = channel_map;
	menu = Menu(tft, radio);
	menu_channels = Menu(tft, channel_map);
	menu_range = Menu(tft, axis_range_min, axis_range_max, &view);
}

void Screen::init() {
	tft->reset();
	tft->begin(0x9341);
	tft->setRotation(3);
	delay(50);
	initial_view();
}

bool Screen::update() {
	if (millis() - update_counter > SCREENUPDATE) {
		update_counter = millis();
		return true;
	}
	return false;
}

char* Screen::navigate(bool left, bool right, bool up, bool down, bool x, bool circle, bool options) {
	char *action = "";
	char *output = "";
	if (view == "options") {
		action = menu.execute(left, right, x, circle);


		if (action == "exit") {
			switch_view("control");
			output = "back to control";
		}

		if (action == "Channels") {
			switch_view("channels");
		}
		else if (action == "Axis Range") {
			switch_view("range");
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
	}
	else if (view == "channels") {
		action = menu_channels.execute(left, right, x, circle);

		if (up) {
			menu_channels.previous();
		}
		else if (down) {
			menu_channels.next();
		}

		if (options) {
			switch_view("control");
			output = "back to control";
		}

		if (circle || action == "exit") {
			switch_view("options");
		}
	}
	else if (view == "range") {
		action = menu_range.execute(left, right, x, circle);
		if (up) {
			menu_range.previous();
		}
		else if (down) {
			menu_range.next();
		}
		if (circle || action == "exit") {
			switch_view("options");
		}
	}
	else if (view == "set_range") {
		action = menu_range.execute(left, right, x, circle);
		if (left || right) {
			menu_range.range_toggle();
		}
		else if (up) {
			menu_range.range_set(+1);
		}
		else if (down) {
			menu_range.range_set(-1);
		}
		if (circle) {
			switch_view("range");
		}
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
		if (type == "success") tft->setTextColor(GREEN);
		else if (type == "warning") tft->setTextColor(YELLOW);
		else if (type == "danger") tft->setTextColor(RED);

		tft->setCursor(150, 40 + 10 * id);
		tft->fillRect(150, 40 + 10 * id, 100, 10, BLACK);
		tft->println(message);
		status_updated = true;
		delay(3);
	}

	if (status_updated) {
		if (id == 0)
			transmitter_status = message;
		else if (id == 2)
			controller_status = message;
	}
}


void Screen::update_analog_axis(uint8_t axis, uint8_t x, uint8_t y, bool force) {

	uint8_t width = 48;
	uint8_t height = 48;

	if (axis == 0) { // Left Stick
		if (x != left_axis_pos[0] || y != left_axis_pos[1] || force) {
			tft->drawLine(11 + left_axis_pos[0] / 255.0*width, 177 + left_axis_pos[1] / 255.0*height, 15 + left_axis_pos[0] / 255.0*width, 177 + left_axis_pos[1] / 255.0 * height, BLACK);
			tft->drawLine(13 + left_axis_pos[0] / 255.0*width, 175 + left_axis_pos[1] / 255.0*height, 13 + left_axis_pos[0] / 255.0*width, 179 + left_axis_pos[1] / 255.0 * height, BLACK);

			tft->drawLine(11 + 24, 177 + 24, 15 + 24, 177 + 24, TEALBLUE);
			tft->drawLine(13 + 24, 175 + 24, 13 + 24, 179 + 24, TEALBLUE);

			tft->drawLine(11 + x / 255.0*width, 177 + y / 255.0*height, 15 + x / 255.0*width, 177 + y / 255.0 * height, ORANGE);
			tft->drawLine(13 + x / 255.0*width, 175 + y / 255.0*height, 13 + x / 255.0*width, 179 + y / 255.0 * height, ORANGE);

			left_axis_pos[0] = x;
			left_axis_pos[1] = y;
		}
	}else if (axis == 1) { // Left Stick
		if (x != right_axis_pos[0] || y != right_axis_pos[1] || force) {
			tft->drawLine(255 + right_axis_pos[0] / 255.0*width, 177 + right_axis_pos[1] / 255.0*height, 259 + right_axis_pos[0] / 255.0*width, 177 + right_axis_pos[1] / 255.0 * height, BLACK);
			tft->drawLine(257 + right_axis_pos[0] / 255.0*width, 175 + right_axis_pos[1] / 255.0*height, 257 + right_axis_pos[0] / 255.0*width, 179 + right_axis_pos[1] / 255.0 * height, BLACK);

			tft->drawLine(255 + 24, 177 + 24, 259 + 24, 177 + 24, TEALBLUE);
			tft->drawLine(257 + 24, 175 + 24, 257 + 24, 179 + 24, TEALBLUE);

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
	delay(5);
}

void Screen::switch_view(String change_to) {
	view = change_to;
	if (view == "control") initial_view();
	else if (view == "options") menu.display("");
	else if (view == "channels") menu_channels.display("");
	else if (view == "range") menu_range.display("");
}

void Screen::update_voltage(float voltage, bool force) {
	if (battery_state != voltage || force) {
		if (voltage < 10) {
			tft->setTextColor(ORANGE);
			tft->setCursor(278, 40);
		}
		else {
			tft->setTextColor(WHITE);
			tft->setCursor(272, 40);
		}
		tft->fillRect(272, 40, 30, 10, BLACK);
		tft->println(voltage);
		battery_state = voltage;
	}
	delay(3);
}

void Screen::draw_plane(double x, double y, double z) {
	// Draws black lines over the old orange lines
	// Calculates the new rotation matrix and the position of the new lines
	// in x and z plane. Afterwards print the new lines on the tft

	if (Rot[0][0] != NULL) {
		for (uint8_t i = 0; i < 12; i++) {
			line[0] = origin[0] + Rot[0][0] * plane_vec[i][0] + Rot[0][1] * plane_vec[i][1] + Rot[0][2] * plane_vec[i][2];
			line[2] = origin[0] + Rot[0][0] * plane_vec[i][3] + Rot[0][1] * plane_vec[i][4] + Rot[0][2] * plane_vec[i][5];

			line[1] = origin[0] + Rot[2][0] * plane_vec[i][0] + Rot[2][1] * plane_vec[i][1] + Rot[2][2] * plane_vec[i][2];
			line[3] = origin[0] + Rot[2][0] * plane_vec[i][3] + Rot[2][1] * plane_vec[i][4] + Rot[2][2] * plane_vec[i][5];
			tft->drawLine(line[0], line[1], line[2], line[3], BLACK);
		}
	}

	double d = sqrt(x * x + y * y + z * z);
	double norm[3] = { x / d, y / d, z / d };
	alpha = acos(norm[0]) + PI/2;
	phi = acos(norm[2]);
	double n[3] = { cos(alpha), sin(alpha), 0 };
	Rot[0][0] = n[0] * n[0] * (1 - cos(phi)) + cos(phi);
	Rot[0][1] = n[0] * n[1] * (1 - cos(phi)) - n[2] * sin(phi);
	Rot[0][2] = n[0] * n[2] * (1 - cos(phi)) + n[1] * sin(phi);

	Rot[1][0] = n[0] * n[1] * (1 - cos(phi)) + n[2] * sin(phi);
	Rot[1][1] = n[1] * n[1] * (1 - cos(phi)) + cos(phi);
	Rot[1][2] = n[1] * n[2] * (1 - cos(phi)) - n[0] * sin(phi);

	Rot[2][0] = n[0] * n[2] * (1 - cos(phi)) - n[1] * sin(phi);
	Rot[2][1] = n[1] * n[2] * (1 - cos(phi)) + n[0] * sin(phi);
	Rot[2][2] = n[2] * n[2] * (1 - cos(phi)) + cos(phi);

	for (uint8_t i = 0; i < 12; i++) {
		line[0] = origin[0] + Rot[0][0] * plane_vec[i][0] + Rot[0][1] * plane_vec[i][1] + Rot[0][2] * plane_vec[i][2];
		line[2] = origin[0] + Rot[0][0] * plane_vec[i][3] + Rot[0][1] * plane_vec[i][4] + Rot[0][2] * plane_vec[i][5];

		line[1] = origin[0] + Rot[2][0] * plane_vec[i][0] + Rot[2][1] * plane_vec[i][1] + Rot[2][2] * plane_vec[i][2];
		line[3] = origin[0] + Rot[2][0] * plane_vec[i][3] + Rot[2][1] * plane_vec[i][4] + Rot[2][2] * plane_vec[i][5];
		tft->drawLine(line[0], line[1], line[2], line[3], ORANGE);
	}
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

	// Draw Left Axis Box
	tft->drawRect(10, 174, 56, 56, WHITE);

	// Draw Right Axis Box
	tft->drawRect(254, 174, 56, 56, WHITE);

	// Draw Battery Level Boxes
	tft->drawRect(302, 10, 8, 20, WHITE);
	tft->drawRect(304, 8, 4, 3, WHITE);

	// Draw Accelerometer box
	tft->drawRect(76, 100, 168, 130, WHITE);


	draw_plane(-4, 0, -9);


	tft->setCursor(304, 40);
	tft->println("V");
}
