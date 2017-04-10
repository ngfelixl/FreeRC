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
	//Parameter params[4];
	//char name[6] = " Min ";
	params = new Parameter[4];
	params[0] = Parameter("Min");
	params[1] = Parameter("Low");
	params[2] = Parameter("High");
	params[3] = Parameter("Max");
	//this->params = params;
	//}
}

char* Option::selectedParam() {
	return this->params[0].getName();
}

char* Option::getType() {
	return this->type;
}

void Option::next() {
	
}

void Option::previous() {

}