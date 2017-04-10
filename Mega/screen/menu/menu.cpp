#include"Menu.h"

Menu::Menu() {}

Menu::Menu(Adafruit_TFTLCD *tft) {
	this->tft = tft;
	options[0] = Option("Channels", "enter");
	options[1] = Option("Axis Range", "enter");
	options[2] = Option("NRF24 PA Level", "select");
	options[3] = Option("Exit", "exit");
	options[0].active = true;
}

void Menu::display(char *type) {
	tft->fillScreen(BLACK);
	tft->setTextColor(ORANGE);
	tft->setTextSize(2);
	tft->setCursor(10, 10);
	tft->println("Options");
	tft->setTextColor(WHITE);
	tft->setTextSize(1.6);
	init_main();
}

void Menu::init_main() {
	uint8_t index = 0;
	for (uint8_t i = 0; i < options_size; i++) {
		tft->setCursor(40, 60 + 30 * i);
		tft->println(options[i].getName());
		Serial.begin(9600);
		Serial.println(options[i].getName());
		tft->drawLine(20, 78 + 30 * i, 300, 78 + 30 * i, DARKGRAY);
		if (options[i].active) {
			index = i;
		}
	}
	setMarker(index);
}

void Menu::next() {
	uint8_t index = getActiveElement();
	options[index].active = false;
	if (index == options_size-1) {
		index = 0;
	}
	else {
		index = index + 1;
	}
	options[index].active = true;
	setMarker(index);
}

void Menu::previous() {
	uint8_t index = getActiveElement();
	options[index].active = false;
	if (index == 0) {
		index = options_size - 1;
	}
	else {
		index = index - 1;
	}
	options[index].active = true;
	setMarker(index);
}

void Menu::setMarker(uint8_t position) {
	tft->fillRect(0, 60, 20, 200, BLACK);
	tft->fillRect(5, 60 + 30 * position, 10, 10, ORANGE);
}

uint8_t Menu::getActiveElement() {
	uint8_t index = -1;
	for (uint8_t i = 0; i < options_size; i++) {
		if (options[i].active) {
			index = i;
			break;
		}
	}
	return index;
}

char* Menu::execute(bool left, bool right, bool x, bool circle) {
	uint8_t active = getActiveElement();
	char *action = "";
	if (active == 2) { // NRF24
		if (left) {
			tft->setTextColor(WHITE);
			tft->setCursor(10, 10);
			tft->println(options[2].selectedParam());
		}
	}
	else if (active == 3) { // Exit
		if (x) {
			//screen->initial_view();
			action = "exit";
		}
	}
	return action;
}