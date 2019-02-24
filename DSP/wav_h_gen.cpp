#include <cstdint>

extern "C" {
#include "spidriver_host.h"
#include "adcdriver_host.h"
}

#include "wav_h_gen.h"

wav_h_gen::wav_h_gen() {
    header = new wav_header;
}

wav_h_gen::wav_h_gen(const wav_h_gen &w) {
    header = new wav_header;
    *header = *w.header;
}

wav_h_gen::~wav_h_gen() {
    delete header;
}

wav_header *wav_h_gen::get_wav_header(int samples_count, int sample_rate) {
    header->sub_chunk_2_size = static_cast<uint32_t>(samples_count * 2);
    header->chunk_size = 36 + header->sub_chunk_2_size;
    header->sample_rate = static_cast<uint32_t>(real_samp_rate(sample_rate));
    header->byte_rate = static_cast<uint32_t>(header->sample_rate * 2);
    return header;
}

int wav_h_gen::real_samp_rate(int sr_const) {
    switch (sr_const) {
        case SAMP_RATE_31250 :
        default:
            return 31250;
        case SAMP_RATE_15625 :
            return 15625;
        case SAMP_RATE_10417 :
            return 10417;
        case SAMP_RATE_5208 :
            return 5208;
        case SAMP_RATE_2604 :
            return 2604;
        case SAMP_RATE_1008 :
            return 1008;
        case SAMP_RATE_504 :
            return 504;
        case SAMP_RATE_400P6 :
            return 381;
        case SAMP_RATE_200P3 :
            return 200;
        case SAMP_RATE_100P2 :
            return 100;
        case SAMP_RATE_59P98 :
            return 59;
        case SAMP_RATE_50 :
            return 50;
    }
}
