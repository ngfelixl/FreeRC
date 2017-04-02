#ifndef SCREEN_H
#define SCREEN_H
#endif

#include <SPFD5408_Adafruit_GFX.h>
#include <SPFD5408_Adafruit_TFTLCD.h>

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4
#define SCREENUPDATE 100 // Update screen every x[ms]

class Screen {
private:
	Adafruit_TFTLCD *tft;
	unsigned int update_counter = 0;

public:
	Screen();
	void init();
	void initial_view();

	void print_servo_default();
	void print_peripheral_status(int id, int color, char *message);
	void update_analog_axis(int axis, float value);
	bool update();
};