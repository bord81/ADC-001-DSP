#ifndef ADC_001_DSP_ALGOS_H
#define ADC_001_DSP_ALGOS_H

#include "mem_man.h"

class algos {
public:
    static void mean_stddev(MemoryData &slot);

    static void mean_stddev_hist(MemoryData &slot);

    static void conv_inp_side(MemoryData &in_slot, MemoryData &resp_slot, MemoryData &out_slot);

    static void conv_outp_side(MemoryData &in_slot, MemoryData &resp_slot, MemoryData &out_slot);

    static void amplify(int strength, MemoryData &in_slot, MemoryData &out_slot);

    static void attenuate(int strength, MemoryData &in_slot, MemoryData &out_slot);

private:
    static void cut_overflow_raw(float *data, size_t size);
};

#endif //ADC_001_DSP_ALGOS_H
