#ifndef PARAMETER_H
#define PARAMETER_H
#endif

class Parameter {
private:
	char *name;

public:
	bool active;


	Parameter();
	Parameter(char *name);
	char* getName();
};