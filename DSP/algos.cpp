#include <cstdio>
#include <cmath>
#include <ctime>

#include "plot.h"
#include "algos.h"

void algos::mean_stddev(MemoryData &slot) {
    float mean = 0.0;
    float var = 0.0;
    float stddev;
    clock_t begin = clock();
    for (int i = 0; i < slot.volts_size; i++) {
        mean += slot.volts[i];
    }
    mean /= slot.volts_size;

    for (int i = 0; i < slot.volts_size; i++) {
        var += pow((slot.volts[i] - mean), 2.0);
    }
    var /= slot.volts_size;
    stddev = static_cast<float>(sqrt(var));
    clock_t end = clock();
    auto elapsed_secs = double(end - begin);
    printf("Mean: %f\n", mean);
    printf("Standard deviation: %f\n", stddev);
    printf("Elapsed time(standard formula): %f\n", elapsed_secs);
    printf("\n");
    //mean and standard deviation with running stats
    float sum = 0.0;
    float sumsqrs = 0.0;
    mean = 0.0;
    begin = clock();
    for (int i = 0; i < slot.volts_size; i++) {
        sum += slot.volts[i];
        sumsqrs += pow(slot.volts[i], 2.0);
        mean = sum / i;
        var = static_cast<float>((sumsqrs - (pow(sum, 2.0) / i)) / (i - 1));
        stddev = static_cast<float>(sqrt(var));
    }
    end = clock();
    elapsed_secs = double(end - begin);
    printf("Running stats mean: %f\n", mean);
    printf("Running stats standard deviation: %f\n", stddev);
    printf("Elapsed time(running stats): %f\n", elapsed_secs);
    printf("\n");
}

void algos::mean_stddev_hist(MemoryData &slot) {
    float mean = 0.0;
    float var;
    float stddev = 0.0;
    float sum = 0.0;
    float sumsqrs = 0.0;
    Plot plot;
    for (int i = 0; i < slot.volts_size; i++) {
        sum += slot.volts[i];
        sumsqrs += pow(slot.volts[i], 2.0);
        mean = sum / i;
        var = static_cast<float>((sumsqrs - (pow(sum, 2.0) / i)) / (i - 1));
        stddev = static_cast<float>(sqrt(var));
    }
    printf("Running stats mean: %f\n", mean);
    printf("Running stats standard deviation: %f\n", stddev);
    printf("Calculating histogram...\n");
    plot.init_p();
    plot.histogram(slot.volts, slot.volts_size);
    plot.close_p();
    printf("\n");
}

void algos::conv_inp_side(MemoryData &in_slot, MemoryData &resp_slot, MemoryData &out_slot) {
    if (mem_man::mem_alloc(out_slot, in_slot.volts_size + resp_slot.volts_size - 1)) {
        printf("Processing convolution...");
        clock_t time = clock();
        for (int i = 0; i < in_slot.volts_size; ++i) {
            for (int j = 0; j < resp_slot.volts_size; ++j) {
                out_slot.volts[i + j] = out_slot.volts[i + j] + in_slot.volts[i] * resp_slot.volts[j];
            }
        }

        time = clock() - time;
        printf("took %lu ticks\n", time);
        cut_overflow_raw(out_slot.volts, out_slot.volts_size);
    } else {
        printf("%s failed to allocate memory\n", __func__);
    }
}

void algos::conv_outp_side(MemoryData &in_slot, MemoryData &resp_slot, MemoryData &out_slot) {
    if (mem_man::mem_alloc(out_slot, in_slot.volts_size + resp_slot.volts_size - 1)) {
        printf("Processing convolution...");
        clock_t time = clock();
        for (int i = 0; i < out_slot.volts_size; ++i) {
            for (int j = 0; j < resp_slot.volts_size; ++j) {
                if (((i - j) > -1) && ((i - j) <= resp_slot.volts_size)) {
                    out_slot.volts[i] = out_slot.volts[i] + resp_slot.volts[j] * in_slot.volts[i - j];
                }
            }
        }
        time = clock() - time;
        printf("took %lu ticks\n", time);
        cut_overflow_raw(out_slot.volts, out_slot.volts_size);
    } else {
        printf("%s failed to allocate memory\n", __func__);
    }
}

void algos::amplify(int strength, MemoryData &in_slot, MemoryData &out_slot) {
    float mult;
    if (strength == 0) {
        mult = 2.0;
    } else {
        mult = strength / 10.0 + 1.0;
    }
    if (mem_man::mem_alloc(out_slot, in_slot.volts_size)) {
        for (int i = 0; i < in_slot.volts_size; ++i) {
            out_slot.volts[i] = in_slot.volts[i] * mult;
        }
        cut_overflow_raw(out_slot.volts, out_slot.volts_size);
    } else {
        printf("%s failed to allocate memory\n", __func__);
    }
}

void algos::attenuate(int strength, MemoryData &in_slot, MemoryData &out_slot) {
    float mult;
    if (strength == 0) {
        mult = 0.5;
    } else {
        mult = 1.0 - strength / 20.0;
    }
    if (mem_man::mem_alloc(out_slot, in_slot.volts_size)) {
        for (int i = 0; i < in_slot.volts_size; ++i) {
            out_slot.volts[i] = in_slot.volts[i] * mult;
        }
        cut_overflow_raw(out_slot.volts, out_slot.volts_size);
    } else {
        printf("%s failed to allocate memory\n", __func__);
    }
}

void algos::cut_overflow_raw(float *data, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (data[i] < -10.0) {
            data[i] = -10.0;
        } else if (data[i] > 10.0) {
            data[i] = 10.0;
        }
    }
}

void algos::low_pass(double cutoff_freq, const int w_func, MemoryData &in_slot, MemoryData &out_slot,
                     MemoryData &kernel_slot, MemoryFilter &filter_slot) {
    if (mem_man::mem_alloc_taps(filter_slot, taps_size) &&
        mem_man::mem_alloc(kernel_slot, taps_size)) {
        wsfirLP(filter_slot.taps, taps_size, w_func, cutoff_freq);
        conv_inp_side(in_slot,
                      mem_man::mem_filter_to_data(filter_slot, kernel_slot), out_slot);
    } else {
        printf("%s failed to allocate memory\n", __func__);
    }
}

void algos::high_pass(double cutoff_freq, const int w_func, MemoryData &in_slot, MemoryData &out_slot,
                      MemoryData &kernel_slot, MemoryFilter &filter_slot) {
    if (mem_man::mem_alloc_taps(filter_slot, taps_size) &&
        mem_man::mem_alloc(kernel_slot, taps_size)) {
        wsfirHP(filter_slot.taps, taps_size, w_func, cutoff_freq);
        conv_inp_side(in_slot,
                      mem_man::mem_filter_to_data(filter_slot, kernel_slot), out_slot);
    } else {
        printf("%s failed to allocate memory\n", __func__);
    }
}

void algos::band_pass(double cutoff_lo, double cutoff_hi, const int w_func, MemoryData &in_slot, MemoryData &out_slot,
                      MemoryData &kernel_slot, MemoryFilter &filter_slot) {
    if (mem_man::mem_alloc_taps(filter_slot, taps_size) &&
        mem_man::mem_alloc(kernel_slot, taps_size)) {
        wsfirBP(filter_slot.taps, taps_size, w_func, cutoff_lo, cutoff_hi);
        conv_inp_side(in_slot,
                      mem_man::mem_filter_to_data(filter_slot, kernel_slot), out_slot);
    } else {
        printf("%s failed to allocate memory\n", __func__);
    }
}

void algos::band_stop(double cutoff_lo, double cutoff_hi, const int w_func, MemoryData &in_slot, MemoryData &out_slot,
                      MemoryData &kernel_slot, MemoryFilter &filter_slot) {
    if (mem_man::mem_alloc_taps(filter_slot, taps_size) &&
        mem_man::mem_alloc(kernel_slot, taps_size)) {
        wsfirBS(filter_slot.taps, taps_size, w_func, cutoff_lo, cutoff_hi);
        conv_inp_side(in_slot,
                      mem_man::mem_filter_to_data(filter_slot, kernel_slot), out_slot);
    } else {
        printf("%s failed to allocate memory\n", __func__);
    }
}

void algos::echo(int delay, float coef, MemoryData &in_slot, MemoryData &out_slot) {
    if (mem_man::mem_alloc(out_slot, in_slot.volts_size + delay)) {
        for (int i = 0; i < in_slot.volts_size; ++i) {
            out_slot.volts[i] += in_slot.volts[i];
            out_slot.volts[i + delay] += in_slot.volts[i] * coef;
        }
    } else {
        printf("%s failed to allocate memory\n", __func__);
    }
}