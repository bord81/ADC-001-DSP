#include <cstring>
#include <cstdint>
#include <sys/stat.h>

extern "C" {
#include "spidriver_host.h"
#include "adcdriver_host.h"
}

#include "wav_h_gen.h"
#include "services.h"

void services::load_file(MemoryData &slot, const char *f_propmt) {
    FILE *data_file;
    char full_fname[100] = {0};
    while (true) {
        char *file_name = enter_fname(f_propmt);
        get_path(full_fname, 100, "samples/", file_name, ".dat");
        data_file = fopen(full_fname, "r");
        if (data_file != nullptr) {
            int vs_24 = fgetc(data_file);
            int vs_16 = fgetc(data_file);
            int vs_8 = fgetc(data_file);
            int vs_lo = fgetc(data_file);
            int sr = fgetc(data_file);
            int size = vs_24 << 24;
            size |= vs_16 << 16;
            size |= vs_8 << 8;
            size |= vs_lo;
            slot.sample_rate = sr;
            alloc_mem(slot, size);
            fread(slot.volts, sizeof(float), static_cast<size_t>(slot.volts_size), data_file);
            fclose(data_file);
            break;
        }
    }
}

char *services::enter_fname(const char *f_propmt) {
    static char file_name[21];
    memset(file_name, 0, sizeof(file_name));
    for (;;) {
        printf(f_propmt);
        fgets(file_name, 20, stdin);
        if ((strlen(file_name) > 2) && (strchr(file_name, 0x20) == nullptr)) {
            char *lf = strchr(file_name, 0xA);
            char *cr = strchr(file_name, 0xD);
            if (lf != nullptr) {
                *lf = '\0';
            }
            if (cr != nullptr) {
                *cr = '\0';
            }
            break;
        }
    }
    return file_name;
}

int services::choose_strength() {
    int ch = 0;
    for (;;) {
        printf("Choose strength(1-0, 10-100pct):");
        ch = getchar();
        if (ch < 48 || ch > 57) {
            continue;
        } else {
            ch -= 48;
            break;
        }
    }
    return ch;
}

void services::get_path(char *dest, size_t dest_size, const char *prefix, const char *name, const char *ext) {
    memset(dest, '\0', dest_size);
    strcpy(dest, prefix);
    strcat(dest, name);
    strcat(dest, ext);
}

void services::read_volts(int x, MemoryData &slot) {
    check_mem(slot, __func__);
    printf("Recording ...");
    alloc_mem(slot, x);
    if (x <= 1024) {
        adc_read_multiple(static_cast<uint32_t>(x), slot.volts);
    } else {
        int y = x / 1024;
        for (int i = 0; i < y; i++) {
            adc_read_multiple(1024, &slot.volts[i * 1024]);
        }
        adc_read_multiple(static_cast<uint32_t>(x - (y * 1024)), &slot.volts[y * 1024]);
    }
    slot.volts_size = x;
    printf(" OK\n");
}

void services::save_to_wav(MemoryData &slot, const char *dest) {
    if (!check_mem(slot, __func__)) {
        return;
    }
    FILE *wav_file;
    wav_h_gen h_gen;
    printf("Saving wav file...");
    auto out_volts = new int16_t[slot.volts_size];
    for (int i = 0; i < slot.volts_size; ++i) {
        out_volts[i] = static_cast<int16_t>(slot.volts[i] * 3276.7);
    }
    wav_file = fopen(dest, "w");
    if (wav_file == nullptr) {
        perror("Following error occured: ");
    } else {
        wav_header *header = h_gen.get_wav_header(slot.volts_size, slot.sample_rate);
        fwrite(header->chunk_id, 4, 1, wav_file);
        fwrite(&header->chunk_size, 4, 1, wav_file);
        fwrite(header->chunk_format, 4, 1, wav_file);
        fwrite(header->sub_chunk_1_id, 4, 1, wav_file);
        fwrite(header->sub_chunk_1_size, 4, 1, wav_file);
        fwrite(header->audio_format, 2, 1, wav_file);
        fwrite(header->num_channels, 2, 1, wav_file);
        fwrite(&header->sample_rate, 4, 1, wav_file);
        fwrite(&header->byte_rate, 4, 1, wav_file);
        fwrite(header->block_align, 2, 1, wav_file);
        fwrite(header->bits_sample, 2, 1, wav_file);
        fwrite(header->sub_chunk_2_id, 4, 1, wav_file);
        fwrite(&header->sub_chunk_2_size, 4, 1, wav_file);
        fwrite(out_volts, sizeof(int16_t), static_cast<size_t>(slot.volts_size), wav_file);
        fflush(wav_file);
        fclose(wav_file);
    }
    delete[] out_volts;
    printf(" OK\n");
}

void services::save_to_dat(MemoryData &slot, const char *dest) {
    if (!check_mem(slot, __func__)) {
        return;
    }
    uint8_t vs_24 = static_cast<uint8_t>(slot.volts_size >> 24);
    uint8_t vs_16 = static_cast<uint8_t>(slot.volts_size >> 16);
    uint8_t vs_8 = static_cast<uint8_t>(slot.volts_size >> 8);
    uint8_t vs_lo = static_cast<uint8_t>(slot.volts_size);
    uint8_t sr = static_cast<uint8_t>(slot.sample_rate);
    struct stat st = {0};
    if (stat("samples", &st) == -1) {
        mkdir("samples", 0666);
    }
    FILE *data_file;
    printf("Saving raw data file...");
    data_file = fopen(dest, "w");
    if (data_file == nullptr) {
        perror("Following error occured: ");
    } else {
        fputc(vs_24, data_file);
        fputc(vs_16, data_file);
        fputc(vs_8, data_file);
        fputc(vs_lo, data_file);
        fputc(sr, data_file);
        fwrite(slot.volts, sizeof(float),
               static_cast<size_t>(slot.volts_size), data_file);
        fflush(data_file);
        fclose(data_file);
    }
    printf(" OK\n");
}

int services::get_real_samp_rate(int sr_const) {
    return wav_h_gen::real_samp_rate(sr_const);
}

void services::display_main() {
    printf("'1' record sample from microphone (slot 1 & file)\n");
    printf("'2' load sample from file to mem slot 1\n");
    printf("'3' load sample from file to mem slot 2\n");
    printf("'4' process sample in mem slot 1\n");
    printf("'5' process sample in mem slot 2\n");
    printf("'6' convolve slots 1 and 2 (input side algorithm, order insensitive)\n");
    printf("'7' convolve slots 1 and 2 (output side algorithm, data in slot 1 must be bigger)\n");
    printf("'8' dump processing result (slot 3) to file\n");
    printf("'9' exit\n");
}

void services::display_algos() {
    printf("'1' Mean and standard deviation\n");
    printf("'2' Histogram, mean and std deviation\n");
    printf("'3' Amplify signal\n");
    printf("'4' Attenuate signal\n");
    printf("'0' back to main menu\n");
}

void services::back_to_main() {
    printf("'0' back to main menu\n");
}

void services::pick_samp_dur() {
    printf("Pick sample duration:\n");
    printf("'1' 1 sec\n");
    printf("'2' 2 sec\n");
    printf("'3' 5 sec\n");
    printf("'4' 10 sec\n");
    printf("'5' back to main menu\n");
}

void services::ch_samp_rate_1() {
    printf("Choose sample rate:\n");
    printf("'1' 31250\n");
    printf("'2' 15625\n");
    printf("'3' 10417\n");
    printf("'4' 5208\n");
    printf("'5' 2604\n");
    printf("'6' 1008\n");
    printf("'7' 504\n");
    printf("'8' 381\n");
    printf("'9' 200\n");
    printf("'n' next page\n");
    printf("'0' back to main menu\n");
}

void services::ch_samp_rate_2() {
    printf("Choose sample rate:\n");
    printf("'1' 100\n");
    printf("'2' 59\n");
    printf("'3' 50\n");
    printf("'p' previous page\n");
    printf("'0' back to main menu\n");
}

void services::set_samp_rate(int rate) {
    adc_set_samplerate(rate);
}

void services::start_adc() {
    adc_config();
    adc_set_samplerate(SAMP_RATE_31250);
    adc_set_chan1();
}

void services::stop_adc(int x) {
    (void) x;
    if (is_started) {
        adc_quit();
    }
}

bool services::check_mem(MemoryData &slot, const char *msg) {
    if (!mem_man::mem_is_ok(slot) || slot.allocated == MEM_FREE) {
        printf("Memory's not ok or empty in services: %s\n", msg);
        return false;
    }
    return true;
}

void services::alloc_mem(MemoryData &slot, size_t size) {
    if (!mem_man::mem_alloc(slot, size)) {
        printf("Memory error in %s\n", __func__);
        stop_adc(1);
        exit(1);
    }
}
