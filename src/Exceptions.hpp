#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <cstdio>
#include <cstdlib>
#include <unistd.h>

class Exceptions
{
  public:
    Exceptions();
    ~Exceptions();

    void die(const char* s);
};

#endif