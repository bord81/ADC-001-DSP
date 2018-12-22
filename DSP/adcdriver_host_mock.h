#ifdef DSP_TEST

#include <cstdint>

#define SAMP_RATE_31250 5
#define SAMP_RATE_15625 6
#define SAMP_RATE_10417 7
#define SAMP_RATE_5208  8
#define SAMP_RATE_2604  9
#define SAMP_RATE_1008  10
#define SAMP_RATE_504   11
#define SAMP_RATE_400P6 12
#define SAMP_RATE_200P3 13
#define SAMP_RATE_100P2 14
#define SAMP_RATE_59P98 15
#define SAMP_RATE_50    16

void adc_config();
void adc_set_samplerate(int rate);
void adc_set_chan1();
void adc_quit();
void adc_read_multiple(uint32_t read_cnt, float *volts);

#endif