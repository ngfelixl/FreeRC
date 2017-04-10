/*#include"Options.h"

Options::Options(Adafruit_TFTLCD *tft) {
	this->tft = tft;
}

Options::Options() {}

void Options::init() {
	//String params[4] = ;
	char *empty[] = { "test" };
	char *params[] = { "31", "32", "23", "13" };
	menu[0].init("Channels", "sub", empty, 0);
	menu[1].init("NRF24 PA Level", "select", params, 1);
	menu[2].init("Exit", "parent", empty, 0);
}

void Options::next() {
	if (active_option_view == "main") {
		for (uint8_t i = 0; i < ARRAY_SIZE(menu); i++) {
			if (menu[i].active) {
				menu[i].active = false;
				if (i == ARRAY_SIZE(menu) - 1)
					menu[0].active = true;
				else
					menu[i + 1].active = true;
				break;
			}
		}
	}
	setMarker();
}

void Options::previous() {
	if (active_option_view == "main") {
		for (uint8_t i = 0; i < ARRAY_SIZE(menu); i++) {
			if (menu[i].active) {
				menu[i].active = false;
				if (i == 0)
					menu[ARRAY_SIZE(menu) - 1].active = true;
				else
					menu[i - 1].active = true;
				break;
			}
		}
	}
	setMarker();
}

void Options::setMarker() {
	tft->fillRect(0, 60, 20, 200, BLACK);
	if (active_option_view == "main") {
		bool active = false;
		for (uint8_t i = 0; i < ARRAY_SIZE(menu); i++) {
			if (menu[i].active) {
				tft->fillRect(5, 60 + 30 * i, 10, 10, ORANGE);
				active = true;
				break;
			}
		}
		if (!active) {
			menu[0].active = true;
			setMarker();
		}
	}
}

void Options::print(char *type) {
	active_option_view = type;
	tft->fillScreen(BLACK);
	tft->setTextColor(ORANGE);
	tft->setTextSize(2);
	tft->setCursor(10, 10);
	tft->println("Options");
	tft->setTextColor(WHITE);
	tft->setTextSize(1.6);

	if (active_option_view == "main") {
		for (uint8_t i = 0; i < ARRAY_SIZE(menu); i++) {
			tft->setCursor(40, 60 + 30 * i);
			tft->println(menu[i].name);
			tft->drawLine(20, 78 + 30 * i, 300, 78 + 30 * i, DARKGRAY);

			//if(menu[i].type == "select")
			if (menu[i].type == "select") {
				tft->setCursor(220, 60 + 30 * i);
				Serial.begin(9600);
				Serial.println(menu[i].name);
				//tft->print(menu[i].params[0]);
			}
		}
		setMarker();
	}
	else {
		tft->setCursor(20, 60);
		tft->println("Option menu not found");
	}
}*/