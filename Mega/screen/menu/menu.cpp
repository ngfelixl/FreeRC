#include"Menu.h"

Menu::Menu() {}

Menu::Menu(Adafruit_TFTLCD *tft, RF24 *radio) {
	// Not default constructor with getting the
	// tfts address and creating the main options menu
	char* menu_type = "main";
	this->tft = tft;
	this->radio = radio;

	options = new Option[4];
	options[0] = Option("Channels", "enter");
	options[1] = Option("Axis Range", "enter");
	options[2] = Option("NRF24 PA Level", "select");
	options[3] = Option("Exit", "exit");
	options[0].active = true;
	options_size = 4;
	title = "Options";
}

Menu::Menu(Adafruit_TFTLCD *tft, uint8_t *channel_map) {
	// Not default constructor with getting the
	// tfts address and creating the options for the channel selection
	char* menu_type = "channel";
	this->tft = tft;
	this->channel_map = channel_map;

	options = new Option[5];
	options[0] = Option("Channel 1", "channel");
	options[1] = Option("Channel 2", "channel");
	options[2] = Option("Channel 3", "channel");
	options[3] = Option("Channel 4", "channel");
	options[4] = Option("Exit", "exit");
	options[0].active = true;
	options_size = 5;
	title = "Channels";
}


void Menu::display(char *type) {
	// Creates the title on the tft display
	tft->fillScreen(BLACK);
	tft->setTextColor(ORANGE);
	tft->setTextSize(2);
	tft->setCursor(10, 10);
	tft->println(title);
	tft->setTextColor(WHITE);
	tft->setTextSize(1);
	init_main();
}

void Menu::init_main() {
	uint8_t index = 0;
	for (uint8_t i = 0; i < options_size; i++) {
		tft->setCursor(30, 60 + 30 * i);
		tft->println(options[i].getName());
		tft->drawLine(20, 78 + 30 * i, 300, 78 + 30 * i, DARKGRAY);
		if (options[i].active) {
			index = i;
		}
	}
	setMarker(index);
	printParameter(-1);
}

void Menu::next() {
	// Same as previous except of the direction
	int8_t index = getActiveElement();
	if (index > -1) {
		options[index].active = false;
		if (index == options_size - 1) {
			index = 0;
		}
		else {
			index = index + 1;
		}
		options[index].active = true;
	} 
	else {
		options[0].active = true;
		index = 0;
	}
	setMarker(index);
}

void Menu::previous() {
	// Set active for current element to false
	// Get previous index (modulo-like) and set
	// Next elements active variable to true
	int8_t index = getActiveElement();
	if (index > -1) {
		options[index].active = false;
		if (index == 0) {
			index = options_size - 1;
		}
		else {
			index = index - 1;
		}
		options[index].active = true;
	}
	else {
		options[0].active = true;
		index = 0;
	}
	setMarker(index);
}

void Menu::setMarker(uint8_t position) {
	tft->fillRect(0, 60, 20, 160, BLACK);
	tft->fillRect(5, 60 + 30 * position, 6, 6, ORANGE);
}

int8_t Menu::getActiveElement() {
	int8_t index = -1;
	for (int8_t i = 0; i < options_size; i++) {
		if (options[i].active) {
			index = i;
			break;
		}
	}
	return index;
}

char* Menu::execute(bool left, bool right, bool x, bool circle) {
	int8_t active = getActiveElement();
	char *action = "";
	if (options[active].getType() == "enter" && x) { // Select channels
		action = options[active].getName();
	}
	else if (options[active].getName() == "NRF24 PA Level") { // NRF24
		if (left) {
			char* state = options[active].previous();
			setRadioLevel(state);
			printParameter(active);
		}
		else if (right) {
			char* state = options[active].next();
			setRadioLevel(state);
			printParameter(active);
		}
	}
	else if (options[active].getType() == "channel") {
		if (left) {
			options[active].previous();
			setChannelMap(active + 1, options[active].getActiveParameter());
			printParameter(active);
		}
		else if (right) {
			options[active].next();
			setChannelMap(active + 1, options[active].getActiveParameter());
			printParameter(active);
		}
	}
	else if (options[active].getType() == "exit" && x) { // Exit
		action = "exit";
	}
	return action;
}

void Menu::printParameter(int8_t id) {
	if (id == -1) {
		for (uint8_t i = 0; i < options_size; i++) {
			if (options[i].getType() == "select" || options[i].getType() == "channel") {
				tft->fillRect(200, 60 + 30 * i, 100, 8, BLACK);
				tft->setCursor(200, 60 + 30 * i);
				tft->setTextColor(WHITE);
				tft->println(options[i].selectedParam());
			}
		}
	}
	else {
		if (options[id].getType() == "select" || options[id].getType() == "channel") {
			tft->fillRect(200, 60 + 30 * id, 100, 8, BLACK);
			tft->setCursor(200, 60 + 30 * id);
			tft->setTextColor(WHITE);
			tft->println(options[id].selectedParam());
		}
	}
}

void Menu::setChannelMap(uint8_t id, uint8_t value) {
	if(id >= 1 && id <= 4)
		channel_map[id] = value;
}

void Menu::setRadioLevel(char *level) {
	if (level == "Min") {
		radio->setPALevel(RF24_PA_MIN);
	}
	else if (level == "Low") {
		radio->setPALevel(RF24_PA_LOW);
	}
	else if (level == "High") {
		radio->setPALevel(RF24_PA_HIGH);
	}
	else if (level == "Max") {
		radio->setPALevel(RF24_PA_MAX);
	}
	delay(20);
}