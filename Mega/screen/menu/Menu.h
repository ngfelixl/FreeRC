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

	char *current_menu = "base";
	Option *options;
	int options_size = 0;

	void init_main();
	int8_t getActiveElement();
	void setMarker(uint8_t position);
	void setRadioLevel(char *level);
	char *title;

public:
	Menu();
	Menu(Adafruit_TFTLCD *tft, RF24 *radio, char* menu_type);
	void display(char *type);
	void next();
	void previous();
	char *execute(bool left, bool right, bool x, bool circle);
	void printParameter(int8_t id);
};