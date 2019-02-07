//
// This class generates WAV binary header
//

#ifndef ADCB_1_WAV_H_GEN_H
#define ADCB_1_WAV_H_GEN_H

#include <memory>

#include "dsp.h"

struct wav_header {
    char chunk_id[4] = {'R', 'I', 'F', 'F'};
    char chunk_format[4] = {'W', 'A', 'V', 'E'};

    char sub_chunk_1_id[4] = {'f', 'm', 't', '\x20'};
    char sub_chunk_1_size[4] = {'\x10', '\0', '\0', '\0'};
    char audio_format[2] = {'\x01', '\0'};
    char num_channels[2] = {'\x01', '\0'};
    char block_align[2] = {'\x02', '\0'};
    char bits_sample[2] = {'\x20', '\0'};

    char sub_chunk_2_id[4] = {'d', 'a', 't', 'a'};

    uint32_t sub_chunk_2_size;
    uint32_t chunk_size;
    uint32_t sample_rate;
    uint32_t byte_rate;
};

class wav_h_gen {
public:
    wav_h_gen();

    std::shared_ptr<wav_header> get_wav_header(int samples_count, int sample_rate);

private:
    std::shared_ptr<wav_header> header;
};


#endif //ADCB_1_WAV_H_GEN_H
