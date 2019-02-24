#ifndef DSP_TEST
#include <cstdio>

#include "plot.h"

void Plot::init_p() {
    handle = gnuplot_init();
    gnuplot_cmd(handle, "set terminal dumb");
}

void Plot::histogram(float *bins, int size) {
    FILE *temp_f = fopen("__temp_hist.pt", "w");
    for (int j = 0; j < size; ++j) {
        fprintf(temp_f, "%f\n", bins[j]);
    }
    fflush(temp_f);
    fclose(temp_f);
    gnuplot_cmd(handle, "binwidth = 0.1");
    gnuplot_cmd(handle, "binstart = -10.0");
    gnuplot_cmd(handle, "set boxwidth binwidth");
    gnuplot_cmd(handle, "hist = 'u (binwidth*(floor(($1-binstart)/binwidth)+0.5)+binstart):(1.0) smooth freq w boxes'");
    gnuplot_cmd(handle, "set xlabel \"Voltage\"");
    gnuplot_cmd(handle, "set ylabel \"Frequency\"");
    gnuplot_cmd(handle, "plot '__temp_hist.pt' @hist");

}

void Plot::close_p() {
    gnuplot_close(handle);
}

#endif