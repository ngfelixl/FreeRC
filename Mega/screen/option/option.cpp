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
	if (name == "NRF24 PA Level") {
		params = new Parameter[4];
		params[0] = Parameter("  1.5 mW");
		params[1] = Parameter("  6.3 mW");
		params[2] = Parameter(" 25.1 mW");
		params[3] = Parameter("100.0 mW");
		params_size = 4;
		params[2].active = true;
	}
	else if (name == "NRF24 Data Rate") {
		params = new Parameter[3];
		params[0] = Parameter("250 kb/s");
		params[1] = Parameter("  1 Mb/s");
		params[2] = Parameter("  2 Mb/s");
		params_size = 3;
		params[0].active = true;
	}
	else if (type == "channel") {
		params = new Parameter[8];
		params[0] = Parameter("Left X");
		params[1] = Parameter("Left X Inv.");
		params[2] = Parameter("Left Y");
		params[3] = Parameter("Left Y Inv.");
		params[4] = Parameter("Right X");
		params[5] = Parameter("Right X Inv.");
		params[6] = Parameter("Right Y");
		params[7] = Parameter("Right Y Inv.");
		params_size = 8;
		params[0].active = true;
	}
}

char* Option::selectedParam() {
	int8_t index = getActiveParameter();
	char *output = "";
	if (index > -1 && index < params_size) {
		output = params[index].getName();
	}
	return output;
}

char* Option::getType() {
	return this->type;
}

char* Option::next() {
	int8_t index = getActiveParameter();
	if (index >= 0) {
		params[index].active = false;
		if (index < params_size - 1) {
			index = index + 1;
		}
		params[index].active = true;
	}
	else {
		params[0].active = true;
	}
	return params[index].getName();
}

char* Option::previous() {
	int8_t index = getActiveParameter();
	if (index >= 0) {
		params[index].active = false;
		if (index > 0) {
			index = index - 1;
		}
		params[index].active = true;
	}
	else {
		index = 0;
		params[0].active = true;
	}
	return params[index].getName();
}

int8_t Option::getActiveParameter() {
	int8_t index = -1;
	for (int8_t i = 0; i < params_size; i++) {
		if (params[i].active) {
			index = i;
			break;
		}
	}
	return index;
}