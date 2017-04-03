#ifndef OPTIONS_H
#define OPTIONS_H
#endif

#define DARKGRAY  int(70/8*2048)  + int(70/4*32)  + int(70/8)
#define BLACK     0x0000
#define ORANGE    int(255/8*2048) + int(120/4*32) + int(0/8)
#define WHITE     0xFFFF

#include<SPFD5408_Adafruit_GFX.h>
#include<SPFD5408_Adafruit_TFTLCD.h>
#include"../option/Option.h"

class Options {
private:
	Option menu[3] = {
		Option(),
		Option(),
		Option()
	};
	uint8_t menu_count = 3;
	Adafruit_TFTLCD *tft;
	char *active_option_view;


	void setMarker();

public:
	Options(Adafruit_TFTLCD &tft);

	void next();
	void previous();

	void print(char *type);
	void init();
};