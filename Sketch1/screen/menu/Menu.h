#ifndef MENU_H
#define MENU_H
#endif

#include<SPFD5408_Adafruit_GFX.h>
#include<SPFD5408_Adafruit_TFTLCD.h>

class Menu {
private:
	Adafruit_TFTLCD *tft;

public:
	Menu(Adafruit_TFTLCD *tft);
	void display(char *type);
};