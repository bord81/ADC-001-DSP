#ifndef FUNCT_H
#define FUNCT_H

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

class DumpConv : public Funct {
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
};

class ChooseSampRate1 : public Funct {
public:
    void exec(int x) override;
};

class ChooseSampRate2 : public Funct {
public:
    void exec(int x) override;
};

class SetSampRateAndRun : public Funct {
public:
    void exec(int x) override;
};

class MainMenu : public Funct {
public:
    void exec(int x) override;
};

class DispAlgos : public Funct {
public:
    void exec(int x) override;
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

class ConvInputSide : public Funct {
public:
    void exec(int x) override;
};

class ConvOutputSide : public Funct {
public:
    void exec(int x) override;
};

class Amplify : public Funct {
public:
    void exec(int x) override;
};

class Attenuate : public Funct {
public:
    void exec(int x) override;
};

#endif

#endif // FUNCT_H