#include"Menu.h"

Menu::Menu() {}

Menu::Menu(Adafruit_TFTLCD *tft, RF24 *radio) {
	// Not default constructor with getting the
	// tfts address and creating the main options menu
	this->tft = tft;
	this->radio = radio;

	options = new Option[5];
	options[0] = Option("Channels", "enter");
	options[1] = Option("Axis Range", "enter");
	options[2] = Option("NRF24 PA Level", "select");
	options[3] = Option("NRF24 Data Rate", "select");
	options[4] = Option("Exit", "exit");
	options[0].active = true;
	options_size = 5;
	title = "Options";
}

Menu::Menu(Adafruit_TFTLCD *tft, uint8_t *channel_map) {
	// Not default constructor with getting the
	// tfts address and creating the options for the channel selection
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

Menu::Menu(Adafruit_TFTLCD *tft, uint8_t *axis_range_min, uint8_t *axis_range_max, String *view) {
	// Not default constructor with getting the
	// tfts address and creating the options for the axis range adjustment
	this->tft = tft;
	this->axis_range_min = axis_range_min;
	this->axis_range_max = axis_range_max;
	this->view = view;

	options = new Option[5];
	options[0] = Option("Channel 1", "range");
	options[1] = Option("Channel 2", "range");
	options[2] = Option("Channel 3", "range");
	options[3] = Option("Channel 4", "range");
	options[4] = Option("Exit", "exit");
	options[0].active = true;
	options_size = 5;
	title = "Axis Range";
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
	else if (options[active].getName() == "NRF24 Data Rate") { // NRF24
		if (left) {
			char* state = options[active].previous();
			setRadioDatarate(state);
			printParameter(active);
		}
		else if (right) {
			char* state = options[active].next();
			setRadioDatarate(state);
			printParameter(active);
		}
	}
	else if (options[active].getType() == "channel") {
		if (left) {
			options[active].previous();
			setChannelMap(active, options[active].getActiveParameter());
			printParameter(active);
		}
		else if (right) {
			options[active].next();
			setChannelMap(active, options[active].getActiveParameter());
			printParameter(active);
		}
	}
	else if (options[active].getType() == "range" && x) {
		channel = active;
		*view = "set_range";
		display_range_adjustment(active);
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

void Menu::display_range_adjustment(uint8_t id) {
	tft->fillScreen(BLACK);
	tft->setCursor(10, 10);
	tft->setTextColor(ORANGE);
	tft->setTextSize(2);
	tft->println(options[id].getName());
	tft->drawLine(30, 220, 290, 220, WHITE);
	tft->drawLine(160, 90, 160, 160, WHITE);
	tft->drawCircle(160, 220, 120, WHITE);
	tft->fillRect(40, 221, 3, 19, BLACK);
	tft->fillRect(278, 221, 3, 19, BLACK);
	min_selected = false;
	range_display_chart(ORANGE);
	min_selected = true;
	range_display_chart(ORANGE);
	range_display_values(false);
}

void Menu::range_set(int8_t value) {
	range_display_values(true);
	range_display_chart(BLACK);
	if (min_selected) {
		axis_range_min[channel] = axis_range_min[channel] + value;
	}
	else {
		axis_range_max[channel] = axis_range_max[channel] + value;
	}
	range_display_chart(ORANGE);
	range_display_values(false);
}

void Menu::range_toggle() {
	min_selected = !min_selected;
	range_display_values(false);
}

void Menu::range_display_chart(uint16_t color) {
	float center = (axis_range_min[channel] + axis_range_max[channel]) / 2.0;
	if(min_selected)
		tft->drawLine(cos(PI - axis_range_min[channel] / 180.0*PI) * 119 + 160, -sin(PI - axis_range_min[channel] / 180.0*PI) * 119 + 220, 160, 220, color);
	else
		tft->drawLine(cos(PI - axis_range_max[channel] / 180.0*PI) * 119 + 160, -sin(PI - axis_range_max[channel] / 180.0*PI) * 119 + 220, 160, 220, color);
	tft->drawLine(cos(PI - center / 180.0*PI) * 59 + 160, -sin(PI - center / 180.0*PI) * 59 + 220, 160, 220, color);
}

void Menu::range_display_values(bool overwrite) {
	if (overwrite) {
		tft->setTextColor(BLACK);
		if (min_selected) {
			tft->setCursor(30, 100);
			tft->println(axis_range_min[channel]);
		}
		else {
			tft->setCursor(250, 100);
			tft->println(axis_range_max[channel]);
		}
	}
	else {
		if (min_selected)
			tft->setTextColor(WHITE);
		else
			tft->setTextColor(DARKGRAY);
		tft->setCursor(30, 100);
		tft->println(axis_range_min[channel]);
		if (min_selected)
			tft->setTextColor(DARKGRAY);
		else
			tft->setTextColor(WHITE);
		tft->setCursor(250, 100);
		tft->println(axis_range_max[channel]);
	}
}

void Menu::setChannelMap(uint8_t id, uint8_t value) {
	if(id >= 0 && id <= 3)
		channel_map[id] = value;
}

void Menu::setRadioLevel(char *level) {
	if (level == "  1.5 mW") {
		radio->setPALevel(RF24_PA_MIN);
	}
	else if (level == "  6.3 mW") {
		radio->setPALevel(RF24_PA_LOW);
	}
	else if (level == " 25.1 mW") {
		radio->setPALevel(RF24_PA_HIGH);
	}
	else if (level == "100.0 mW") {
		radio->setPALevel(RF24_PA_MAX);
	}
	delay(20);
}

void Menu::setRadioDatarate(char *rate) { 
	if (rate == "250 kb/s") {
		radio->setDataRate(RF24_250KBPS);
	}
	else if (rate == "  1 Mb/s") {
		radio->setDataRate(RF24_1MBPS);
	}
	else if (rate == "  2 Mb/s") {
		radio->setDataRate(RF24_2MBPS);
	}
}