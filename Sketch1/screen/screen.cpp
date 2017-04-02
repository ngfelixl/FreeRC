#include"Screen.h"

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Screen::Screen() {
	tft = new Adafruit_TFTLCD(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
}

void Screen::init() {
	tft->reset();
	tft->begin(0x9341);
	tft->setRotation(3);
	initial_view();
	print_servo_default();
}

bool Screen::update() {
	if (millis() - update_counter > SCREENUPDATE) {
		update_counter = millis();
		return true;
	}
	else
		return false;
}

void Screen::print_peripheral_status(int id, int color, char *message) {
	tft->setTextColor(color);
	tft->setCursor(150, 40 + 10 * id);
	tft->fillRect(150, 40 + 10 * id, 320, 10, BLACK);
	tft->println(message);
}

void Screen::print_servo_default() {
	// Servo field dim 175x130, pos 67,100
	tft->drawLine(67 + 175 / 2, 120, 67 + 175 / 2, 210, GREEN);
	tft->drawLine(67 + 20, 100 + 130 / 2, 67 + 175 - 20, 100 + 130 / 2, GREEN);
}

void Screen::update_analog_axis(int axis, float value) {
	// axis 0 -> left x
	// axis 1 -> left y
	// axis 2 -> right x
	// axis 3 -> right y
	int i = axis%2;
	int j = floor(axis / 2);
	tft->fillRect(261 + i * 30, 101 + j * 75 + (53 - (value / 255) * 53), 18, (value / 255) * 53, BLUE);
	tft->fillRect(261 + i * 30, 101 + j * 75, 18, 53 - (value / 255) * 53, BLACK);
}

void Screen::initial_view() {
	tft->fillScreen(BLACK);
	tft->setCursor(10, 10);
	tft->setTextColor(RED);
	tft->setTextSize(2);
	tft->println("RC Flight Control");
	tft->setTextColor(WHITE);
	tft->setTextSize(1);

	// Add peripherals entry under title
	tft->setCursor(10, 40);
	tft->println("NRF24L01+");
	tft->setCursor(10, 50);
	tft->println("USB Host Shield");
	tft->setCursor(10, 60);
	tft->println("PS4 Controller");

	// Add subsection titles
	tft->setTextColor(MAGENTA);
	tft->setCursor(10, 75);
	tft->println("Nano Feedback");
	tft->drawLine(10, 85, 242, 85, MAGENTA);
	tft->setCursor(260, 75);
	tft->println("DS4");
	tft->drawLine(260, 85, 310, 85, MAGENTA);
	tft->setTextColor(WHITE);

	// Draw Motor Status box
	tft->setCursor(10, 90);
	tft->println("Motor");
	tft->drawRect(10, 100, 25, 130, WHITE);
	tft->setTextColor(CYAN);
	tft->setCursor(40, 95);
	tft->println("100");
	tft->setCursor(40, 160);
	tft->println("50");
	tft->setCursor(40, 225);
	tft->println("0");
	tft->setTextColor(WHITE);

	// Draw Global Servo Status box
	tft->setCursor(67, 90);
	tft->println("Servo");
	tft->drawRect(67, 100, 175, 130, WHITE);

	// Draw PS4 Status box
	for (int i = 0; i<2; i++) {
		for (int j = 0; j<2; j++) {
			tft->drawRect(260 + i * 30, 100 + j * 75, 20, 55, WHITE);
			tft->drawLine(256 + i * 30, 101 + 26 + j * 75, 259 + i * 30, 101 + 26 + j * 75, WHITE);
			//tft->drawLine(256 + i * 30, 101 + 53 / 2 + ((float)ds4deadzone / 255) * 53 + j * 75, 259 + i * 30, 101 + 53 / 2 + ((float)ds4deadzone / 255) * 53 + j * 75, RED);
			//tft->drawLine(256 + i * 30, 101 + 53 / 2 - ((float)ds4deadzone / 255) * 53 + j * 75, 259 + i * 30, 101 + 53 / 2 - ((float)ds4deadzone / 255) * 53 + j * 75, RED);
		}
	}
}
