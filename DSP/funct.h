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
    void exec(int x) override;
};

class LoadFile : public Funct {
public:
    void exec(int x) override;
};

/* Exit from program */
class ExitFromProg : public Funct {
public:
    void exec(int x) override;

private:
    Init init;
};

class ToArrayAndRun : public Funct {
public:
    void exec(int x) override;

private:
    Init init;
};

class ChooseSampRate1 : public Funct {
public:
    void exec(int x) override;
};

class ChooseSampRate2 : public Funct {
public:
    void exec(int x) override;
};

class SetSampRate : public Funct {
public:
    void exec(int x) override;
};

class MainMenu : public Funct {
public:
    void exec(int x) override;
};

class ToFile : public Funct {
public:
    void exec(int x) override;

private:
    Init init;
};

class MeanAndStdDev : public Funct {
public:
    void exec(int x) override;
};

#ifndef DSP_TEST

class HistMeanAndStdDev : public Funct {
public:
    void exec(int x) override;
};

class ConvertToWav : public Funct {
public:
    void exec(int x) override;
};

#endif

#endif // FUNCT_H