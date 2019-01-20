#ifndef FUNCT_H
#define FUNCT_H

#include <cstdio>
#include "init.h"

class Funct {
public:
    virtual void exec(int x) = 0;

    virtual ~Funct() = 0;
};

inline Funct::~Funct() {}

typedef Funct *FunctPtr;


class MicInput : public Funct {
public:
    virtual void exec(int x);
};

class LoadFile : public Funct {
public:
    LoadFile(FILE *f) : h_stream(f) {}

    virtual void exec(int x);

private:
    FILE *h_stream;
};

/* Exit from program */
class ExitFromProg : public Funct {
public:
    virtual void exec(int x);

private:
    Init init;
};

class ToArrayAndRun : public Funct {
public:
    virtual void exec(int x);

private:
    Init init;
};

class ChooseSampRate1 : public Funct {
public:
    virtual void exec(int x);
};

class ChooseSampRate2 : public Funct {
public:
    virtual void exec(int x);
};

class SetSampRate : public Funct {
public:
    virtual void exec(int x);
};

class MainMenu : public Funct {
public:
    virtual void exec(int x);
};

class ToFile : public Funct {
public:
    ToFile(FILE *f) : h_stream(f) {}

    virtual void exec(int x);

private:
    FILE *h_stream;
    Init init;
};

class MeanAndStdDev : public Funct {
public:
    virtual void exec(int x);
};

#ifndef DSP_TEST

class HistMeanAndStdDev : public Funct {
public:
    virtual void exec(int x);
};

#endif

#endif // FUNCT_H