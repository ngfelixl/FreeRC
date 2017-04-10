#include"Parameter.h"

Parameter::Parameter() {}

Parameter::Parameter(char *name) {
	this->name = name;
}

char* Parameter::getName() {
	return name;
}