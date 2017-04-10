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
	params[0].active = true;
	//this->params = params;
	//}
}

char* Option::selectedParam() {
	int8_t index = getActiveParameter();
	char *output = "";
	if(index > -1 && index < ARRAY_SIZE(params))
		output = params[index].getName();
	Serial.begin(9600);
	Serial.println(index);
	return output;
}

char* Option::getType() {
	return this->type;
}

void Option::next() {
	int8_t index = getActiveParameter();
	if (index >= 0) {
		params[index].active = false;
		if (index < ARRAY_SIZE(params) - 1) {
			index = index + 1;
		}
		params[index].active = true;
	}
	else {
		params[0].active = true;
	}
}

void Option::previous() {

}

int8_t Option::getActiveParameter() {
	int8_t index = -1;
	for (int8_t i = 0; i < ARRAY_SIZE(params); i++) {
		if (params[i].active) {
			index = i;
			break;
		}
	}
	return index;
}