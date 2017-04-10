#include"Option.h"

Option::Option() {}
Option::Option(char *name, char *type) {
	this->name = name;
	this->type = type;
	initialize();
}

char* Option::getName() {
	return this->name;
}

void Option::initialize() {
	//if (name == "NRF24 PA Level") {
		char *params[4] = { "Min", "Low", "High", "Max" };
		this->params = params;
	//}
}

char* Option::selectedParam() {
	return this->params[0];
}

char* Option::getType() {
	return this->type;
}

void Option::next() {

}

void Option::previous() {

}