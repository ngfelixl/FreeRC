#ifndef MENU_H
#define MENU_H
#endif

#define DARKGRAY  int(70/8*2048)  + int(70/4*32)  + int(70/8)
#define BLACK     0x0000
#define ORANGE    int(255/8*2048) + int(120/4*32) + int(0/8)
#define WHITE     0xFFFF

#include<SPFD5408_Adafruit_GFX.h>
#include<SPFD5408_Adafruit_TFTLCD.h>

class Menu {
private:
	Adafruit_TFTLCD *tft;
	char *current_menu = "base";

public:
	Menu();
	Menu(Adafruit_TFTLCD *tft);
	void display(char *type);
};