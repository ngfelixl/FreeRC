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
		params[0] = Parameter("Min");
		params[1] = Parameter("Low");
		params[2] = Parameter("High");
		params[3] = Parameter("Max");
		params_size = 4;
		params[2].active = true;
	}
	else if (type == "channel") {
		params = new Parameter[8];
		params[0] = Parameter("Left X");
		params[1] = Parameter("- Left X");
		params[2] = Parameter("Left Y");
		params[3] = Parameter("- Left Y");
		params[4] = Parameter("Right X");
		params[5] = Parameter("- Right X");
		params[6] = Parameter("Right Y");
		params[7] = Parameter("- Right Y");
		params_size = 8;

		if (name == "Channel 1") {
			params[2].active = true;
		}
		else if (name == "Channel 2") {
			params[5].active = true;
		}
		else {
			params[0].active = true;
		}
	}
	//this->params = params;
	//}
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