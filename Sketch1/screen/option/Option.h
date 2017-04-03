#ifndef OPTION_H
#define OPTION_H
#endif

#include<SPFD5408_Adafruit_GFX.h>
#include<SPFD5408_Adafruit_TFTLCD.h>

class Option {
private:

public:
	char *name;
	char *type; // select, sub, parent
	String *params;
	uint8_t params_count;
	uint8_t selected;

	bool active = false;

	Option();

	void display();
	void init(char *name, char *type, String *params, uint8_t params_count, uint8_t selected);
	String get();
};