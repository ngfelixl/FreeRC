#include"Option.h"

Option::Option() {}

void Option::display() {

}

void Option::init(char *name, char *type, String *params, uint8_t selected) {
	this->name = name;
	this->params = params;
	this->type = type;
	this->selected = selected;
}

void Option::operate(char *operation) {
	if (this->type == "select") {
		if (operation == "next") {
			if (selected < (ARRAY_SIZE(params) - 1))
				selected = selected + 1;
		}
		else if(operation == "previous") {
			if (selected > 0)
				selected = selected - 1;
		}
	}
}