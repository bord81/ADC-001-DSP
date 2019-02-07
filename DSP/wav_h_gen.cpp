#include "wav_h_gen.h"

wav_h_gen::wav_h_gen() {
    header = std::make_shared<wav_header>();
}

std::shared_ptr<wav_header> wav_h_gen::get_wav_header(int samples_count, int sample_rate) {
    header->sub_chunk_2_size = static_cast<uint32_t>(samples_count * 2);
    header->chunk_size = 36 + header->sub_chunk_2_size;
    switch (sample_rate) {
        case SAMP_RATE_31250 :
            header->sample_rate = 31250;
            break;
        case SAMP_RATE_15625 :
            header->sample_rate = 15625;
            break;
        case SAMP_RATE_10417 :
            header->sample_rate = 10417;
            break;
        case SAMP_RATE_5208 :
            header->sample_rate = 5208;
            break;
        case SAMP_RATE_2604 :
            header->sample_rate = 2604;
            break;
        case SAMP_RATE_1008 :
            header->sample_rate = 1008;
            break;
        case SAMP_RATE_504 :
            header->sample_rate = 504;
            break;
        case SAMP_RATE_400P6 :
            header->sample_rate = 381;
            break;
        case SAMP_RATE_200P3 :
            header->sample_rate = 200;
            break;
        case SAMP_RATE_100P2 :
            header->sample_rate = 100;
            break;
        case SAMP_RATE_59P98 :
            header->sample_rate = 59;
            break;
        case SAMP_RATE_50 :
            header->sample_rate = 50;
            break;
    }
    header->byte_rate = static_cast<uint32_t>(header->sample_rate * 2);
    return header;
}
