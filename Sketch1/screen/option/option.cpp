#include"Option.h"

Option::Option() {}

void Option::display() {

}

void Option::init(char *name, char *type, String *params, uint8_t params_count, uint8_t selected) {
	this->name = name;
	this->params = params;
	this->params_count = params_count;
	this->type = type;
	this->selected = selected;
}

String Option::get() {
	return params[selected];
}