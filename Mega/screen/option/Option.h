#ifndef OPTION_H
#define OPTION_H
#endif

#include"parameter/Parameter.h"
#include<Arduino.h>

#define ARRAY_SIZE(X) sizeof(X)/sizeof(X[0])

class Option {
private:
	char *name;
	char *type;
	Parameter *params;
	uint8_t params_size;

	void initialize();


public:
	bool active = false;

	Option();
	Option(char *name, char *type);
	char* getName();
	char* getType();
	char* selectedParam();
	int8_t getActiveParameter();

	char* next();
	char* previous();
};