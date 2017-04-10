#ifndef OPTION_H
#define OPTION_H
#endif

#define ARRAY_SIZE(X) sizeof(X)/sizeof(X[0])

//#include<SPFD5408_Adafruit_GFX.h>
//#include<SPFD5408_Adafruit_TFTLCD.h>

class Option {
private:
	char *name;

public:
	char *type; // select, sub, exit
	//uint8_t selected;
	// typedef void(*callback) (void);
	// callback *func;
	bool active = false;

	Option();
	Option(char *name, char *type);

	char* getName();

	//void display();
	//void init(char *name, char *type, String *params, uint8_t selected);
	//void init(char *name, char *type);
	//void operate(char *operation);
};