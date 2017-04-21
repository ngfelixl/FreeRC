#ifndef MENU_H
#define MENU_H
#endif

#define DARKGRAY  int(70/8*2048)  + int(70/4*32)  + int(70/8)
#define BLACK     0x0000
#define ORANGE    int(255/8*2048) + int(120/4*32) + int(0/8)
#define WHITE     0xFFFF

#define ARRAY_SIZE(X) sizeof(X)/sizeof(X[0])

#include<Arduino.h>
#include<SPFD5408_Adafruit_GFX.h>
#include<SPFD5408_Adafruit_TFTLCD.h>
#include"../option/Option.h"
#include "RF24.h"

class Menu {
private:
	Adafruit_TFTLCD *tft;
	RF24 *radio;
	uint8_t *channel_map;
	uint8_t *axis_range_min;
	uint8_t *axis_range_max;

	char *current_menu = "base";
	String *view;
	Option *options;
	int options_size = 0;
	bool min_selected = true; // true = min, false = max
	uint8_t channel;

	void init_main();
	int8_t getActiveElement();
	void setMarker(uint8_t position);
	void setRadioLevel(char *level);
	void setRadioDatarate(char *rate);
	void setChannelMap(uint8_t id, uint8_t value);
	char *title;
	void display_range_adjustment(uint8_t id);
	void range_display_values(bool overwrite);
	void range_display_chart(uint16_t color);

public:
	Menu();
	Menu(Adafruit_TFTLCD *tft, RF24 *radio);
	Menu(Adafruit_TFTLCD *tft, uint8_t *channel_map);
	Menu(Adafruit_TFTLCD *tft, uint8_t *axis_range_min, uint8_t *axis_range_max, String *view);
	void display(char *type);
	void next();
	void previous();
	char *execute(bool left, bool right, bool x, bool circle);
	void printParameter(int8_t id);
	void range_toggle();
	void range_set(int8_t value);
};