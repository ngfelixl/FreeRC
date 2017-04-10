#ifndef OPTIONS_H
#define OPTIONS_H
#endif

#define DARKGRAY  int(70/8*2048)  + int(70/4*32)  + int(70/8)
#define BLACK     0x0000
#define ORANGE    int(255/8*2048) + int(120/4*32) + int(0/8)
#define WHITE     0xFFFF

#define ARRAY_SIZE(X) sizeof(X)/sizeof(X[0])

#include<SPFD5408_Adafruit_GFX.h>
#include<SPFD5408_Adafruit_TFTLCD.h>
#include"../option/Option.h"

//#include <StandardCplusplus.h>
//#include <serstream>
//#include <string>
//#include <vector>
//#include <iterator>

class Options {
private:
	/*typedef struct params {
		int id;
		String name;
	}  parameter_t;

	typedef struct option{
		int id;
		String name;
		parameter_t params[];
	} options_t;

	options_t *menu;*/
	Option menu[3];
	Adafruit_TFTLCD *tft;
	char *active_option_view;


	void setMarker();

public:
	Options();
	Options(Adafruit_TFTLCD *tft);

	void next();
	void previous();

	void print(char *type);
	void init();
};