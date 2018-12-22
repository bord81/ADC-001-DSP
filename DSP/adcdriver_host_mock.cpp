#ifdef DSP_TEST

#include <random>
#include "adcdriver_host_mock.h"

int test_adc_config = 0;
int test_adc_samprate = 0;
int test_adc_chan1 = 0;
int test_adc_quit = 0;

void adc_config()
{
    test_adc_config = 1;
}

void adc_set_samplerate(int rate)
{
    test_adc_samprate = rate;
}
void adc_set_chan1()
{
    test_adc_chan1 = 1;
}
void adc_quit()
{
    test_adc_quit = 1;
}
void adc_read_multiple(uint32_t read_cnt, float *volts)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(1.0, 2.0);
    for (uint32_t i = 0; i < read_cnt; i++) {
        *(volts + i) = dis(gen);
    }
}

#endif