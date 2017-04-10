#ifndef SCREEN_H
#define SCREEN_H
#endif

#include <SPFD5408_Adafruit_GFX.h>
#include <SPFD5408_Adafruit_TFTLCD.h>
#include <stdlib.h>
#include "options/Options.h"

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4
#define SCREENUPDATE 100 // Update screen every x[ms]

#define BLACK     0x0000
#define BLUE      0x001F
#define RED       int(238/8*2048) + int(0) + int(0)
#define GREEN     0x07E0
#define CYAN      0x07FF
#define MAGENTA   0xF81F
#define YELLOW    0xFFE0
#define WHITE     0xFFFF
#define DARKGREEN int(255/8*2048) + int(255/4*32) + int(0/8)
#define DARKGRAY  int(70/8*2048)  + int(70/4*32)  + int(70/8)
#define LIGHTGRAY int(180/8*2048) + int(180/4*32) + int(180/8)
#define ORANGE    int(255/8*2048) + int(120/4*32) + int(0/8)
#define TEALBLUE  int(56/8*2048)  + int(142/4*32) + int(142/8)
#define TURQUISE  int(0/8*2048)  + int(245/4*32) + int(255/8)

class Screen {
private:
	Adafruit_TFTLCD *tft;
	unsigned int update_counter = 0;
	short option_selected = 0;
	//Options menu;

public:
	String view = "control";
	Options menu;

	Screen();
	void init();
	void initial_view();
	void options_view();
	void options_navigate(char* direction);

	void switch_marker();
	void switch_view(String view);
	void print_servo_default();
	void print_peripheral_status(int id, char* type, char *message);
	void update_analog_axis(int axis, float value);
	bool update();
	void update_motor(float value);

};