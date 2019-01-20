#ifndef INIT_H
#define INIT_H

class Init {
public:
    Init();

    void start_adc();

    void stop_adc(int status);

private:
    static bool is_started;
};

#endif