#ifndef DSP_TEST
#ifndef PLOT_H
#define PLOT_H

extern "C" {
#include "gnuplot_i.h"
}

class Plot {
public:
    virtual void init_p();

    virtual void histogram(float *bins, int size);

    virtual void close_p();

private:
    gnuplot_ctrl *handle;
};

#endif //PLOT_H
#endif
