#ifndef ADC_001_DSP_SERVICES_H
#define ADC_001_DSP_SERVICES_H

#include "mem_man.h"

class services {
public:
    services() {
        is_started = true;
    }

    void read_volts(int x, MemoryData &slot);

    char *enter_fname(const char *f_propmt);

    void get_path(char *dest, size_t dest_size, const char *prefix, const char *name, const char *ext);

    void load_file(MemoryData &slot, const char *f_propmt);

    void display_algos();

    void display_main();

    void back_to_main();

    void pick_samp_dur();

    void ch_samp_rate_1();

    void ch_samp_rate_2();

    void set_samp_rate(int rate);

    int get_real_samp_rate(int sr_const);

    static void start_adc();

    void stop_adc(int status);

    void save_to_wav(MemoryData& slot, const char *dest);

    void save_to_dat(MemoryData& slot, const char *dest);

    int choose_strength();

    double choose_cutoff_freq(const char *prompt, int sampl_rate);

    int choose_window_func(const char *prompt);

private:
    void alloc_mem(MemoryData& slot, size_t size);
    bool check_mem(MemoryData& slot, const char *msg);
    bool is_started;
};


#endif //ADC_001_DSP_SERVICES_H
