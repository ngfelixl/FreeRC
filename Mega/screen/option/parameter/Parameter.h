#ifndef PARAMETER_H
#define PARAMETER_H
#endif

#include<Arduino.h>

class Parameter {
private:
	char *name;

public:
	bool active = false;

	Parameter();
	Parameter(char *name);
	char* getName();
};