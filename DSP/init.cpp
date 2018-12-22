#include "dsp.h"

#ifdef DSP_TEST
#include <cstring>
#include "doctest.h"

extern int test_adc_samprate;

#endif


bool Init::is_started = false;

Init::Init() {}

void Init::start_adc()
{
    if (!is_started) {
        adc_config();
	    adc_set_samplerate(SAMP_RATE_31250);
        adc_set_chan1();
        is_started = true;
    }
}

void Init::stop_adc(int status)
{
    if (is_started) {
        adc_quit();
    }
    exit(status);
}

#ifdef DSP_TEST

extern int test_adc_config;
extern int test_adc_samprate;
extern int test_adc_quit;
extern int test_adc_chan1;

static Init init_main;

TEST_CASE("DSP test. Init::start_adc") {
    CHECK(test_adc_config == 0);
    CHECK(test_adc_chan1 == 0);

    init_main.start_adc();

    CHECK(test_adc_config == 1);
    CHECK(test_adc_samprate == SAMP_RATE_31250);
    CHECK(test_adc_chan1 == 1);
}

TEST_CASE("DSP test. Init::stop_adc") {
    CHECK(test_adc_quit == 0);

    init_main.stop_adc(0);

    CHECK(test_adc_quit == 1);
}

#endif
