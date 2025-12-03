#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <unistd.h>
#include <cstdio>
#include <cstdlib>

class Exceptions
{
public:
	Exceptions();
	~Exceptions();

	void die(const char *s);
};

#endif