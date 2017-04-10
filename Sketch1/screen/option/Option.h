#ifndef OPTION_H
#define OPTION_H
#endif

#define ARRAY_SIZE(X) sizeof(X)/sizeof(X[0])

#include<SPFD5408_Adafruit_GFX.h>
#include<SPFD5408_Adafruit_TFTLCD.h>

class Option {
private:

public:
	String name;
	String type; // select, sub, parent
	char **params;
	uint8_t selected;
	// typedef void(*callback) (void);
	// callback *func;
	bool active = false;

	Option();

	void display();
	//void init(char *name, char *type, String *params, uint8_t selected);
	void init(String name, String type, char *params[], uint8_t selected);
	void operate(char *operation);
};