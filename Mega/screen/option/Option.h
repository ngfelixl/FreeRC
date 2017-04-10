#ifndef OPTION_H
#define OPTION_H
#endif

#define ARRAY_SIZE(X) sizeof(X)/sizeof(X[0])

class Option {
private:
	char *name;

public:
	char *type;
	bool active = false;

	Option();
	Option(char *name, char *type);

	char* getName();
};