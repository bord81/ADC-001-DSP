#include "dsp.h"

bool Init::is_started = false;

Init::Init() {}

void Init::start_adc() {
    if (!is_started) {
        adc_config();
        adc_set_samplerate(SAMP_RATE_31250);
        adc_set_chan1();
        is_started = true;
    }
}

void Init::stop_adc(int status) {
    if (is_started) {
        adc_quit();
    }
    exit(status);
}
