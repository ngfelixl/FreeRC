#include"Menu.h"

Menu::Menu() {}

Menu::Menu(Adafruit_TFTLCD *tft) {
	this->tft = tft;
	options[0] = Option("Channels", "enter");
	options[1] = Option("Axis Range", "enter");
	options[2] = Option("NRF24 PA Level", "select");
	options[3] = Option("Exit", "exit");
	
}

void Menu::display(char *type) {
	//active_option_view = type;
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
	for (uint8_t i = 0; i < ARRAY_SIZE(options); i++) {
		tft->setCursor(40, 60 + 30 * i);
		tft->println(options[i].getName());
		Serial.begin(9600);
		Serial.println(options[i].getName());
		tft->drawLine(20, 78 + 30 * i, 300, 78 + 30 * i, DARKGRAY);
	}
}

void Menu::next() {

}

void Menu::previous() {

}