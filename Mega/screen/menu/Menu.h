#ifndef MENU_H
#define MENU_H
#endif

#define DARKGRAY  int(70/8*2048)  + int(70/4*32)  + int(70/8)
#define BLACK     0x0000
#define ORANGE    int(255/8*2048) + int(120/4*32) + int(0/8)
#define WHITE     0xFFFF

#define ARRAY_SIZE(X) sizeof(X)/sizeof(X[0])


#include<SPFD5408_Adafruit_GFX.h>
#include<SPFD5408_Adafruit_TFTLCD.h>
#include"../option/Option.h"

class Menu {
private:
	Adafruit_TFTLCD *tft;
	char *current_menu = "base";
	void init_main();
	Option options[4];

public:
	Menu();
	Menu(Adafruit_TFTLCD *tft);
	void display(char *type);
	void next();
	void previous();
};