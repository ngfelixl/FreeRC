#ifndef SCREEN_H
#define SCREEN_H
#endif

#include <SPFD5408_Adafruit_GFX.h>
#include <SPFD5408_Adafruit_TFTLCD.h>
#include <stdlib.h>
#include "menu/Menu.h"
#include "RF24.h"

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
	RF24 *radio;
	uint8_t *channel_map;

	unsigned int update_counter = 0;
	char *controller_status = "";
	char *transmitter_status = "";
	uint8_t left_axis_pos[2] = { 0, 0 };
	uint8_t right_axis_pos[2] = { 0,0 };
	uint8_t ds4_battery_state = 0;
	uint8_t motor_state = -1;
	float battery_state = 0.0;


	int16_t plane_vec[12][6] = {
		{ 0, 30, 0, 0, -58, 0 }, // Rumpf
		{ 0, 30, -4, 0, -70, -4 }, // Rumpf
		{ -60, 0, 0, 60, 0, 0 }, // Flügel front
		{ -60, -10, 0, 60, -10, 0 }, // Flügel hinten
		{ -20, -70, 18, 20, -70, 18 }, // Side rudder
		{ 0, -70, -4, 0, -70, 18 }, // Height rudder
		{ 60, 0, 0, 60, -10, 0 },
		{ -60, 0, 0, -60, -10, 0},
		{ -20, -65, 18, 20, -65, 18},
		{ -20, -65, 18, -20, -70, 18},
		{ 20, -65, 18, 20, -70, 18},
		{ 0, -65, 18, 0, -58, 0}
	};
	int16_t line[4];
	int16_t origin[2] = { 160, 165 };
	double phi = 0, alpha = 0;
	double Rot[3][3];

public:
	String view = "control";
	Menu menu;
	Menu menu_channels;

	Screen(RF24 *radio, uint8_t *channel_map);
	void init();
	void initial_view();
	char* navigate(bool left, bool right, bool up, bool down, bool x, bool circle, bool options);
	//void options_navigate(char* direction);

	//void switch_marker();
	void switch_view(String view);
	void print_peripheral_status(int id, char* type, char *message, bool force);
	void update_analog_axis(uint8_t axis, uint8_t x, uint8_t y, bool force);
	bool update();
	void update_motor(uint8_t motor);
	void update_battery(uint8_t ds4_battery, bool force);
	void update_voltage(float voltage, bool force);
	void draw_plane(double x, double y, double z);
};