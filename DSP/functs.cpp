#include <cmath>
#include <ctime>
#include <memory>
#include "dsp.h"
#include "funct.h"
#include "wav_h_gen.h"

#include "plot.h"

static float volts[12288];
static int volts_size;
static int current_samp_rate = SAMP_RATE_31250;
static char file_name[21];
static FILE *data_file;

static const char *load_f = "Enter filename to load (max 20 chars): \n";
static const char *convert_open_f = "Enter source filename to convert (max 20 chars): \n";
static const char *convert_save_f = "Enter WAV filename destination (max 20 chars): \n";
static const char *save_f = "Enter filename to save (max 20 chars): \n";

static inline void display_algos();

static inline void display_main();

static inline void back_to_main();

static inline void read_volts(int x, Init &init);

static inline void enter_fname(const char *f_propmt);

static inline void load_file(const char *f_propmt);

void MicInput::exec(int x) {
    (void) x;
    printf("Pick number of samples:\n");
    printf("'1' 512\n");
    printf("'2' 1024\n");
    printf("'3' 2048\n");
    printf("'4' 4096\n");
    printf("'5' 8192\n");
    printf("'6' 12288\n");
    printf("'7' back to main menu\n");
}


void LoadFile::exec(int x) {
    (void) x;
    load_file(load_f);
    display_algos();
}

void ExitFromProg::exec(int x) {
    init.stop_adc(x);
}

void ToArrayAndRun::exec(int x) {
    read_volts(x, init);
    display_algos();
}


void ChooseSampRate1::exec(int x) {
    (void) x;
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


void ChooseSampRate2::exec(int x) {
    (void) x;
    printf("Choose sample rate:\n");
    printf("'1' 100\n");
    printf("'2' 59\n");
    printf("'3' 50\n");
    printf("'p' previous page\n");
    printf("'0' back to main menu\n");
}


void SetSampRate::exec(int x) {
    current_samp_rate = x;
    adc_set_samplerate(x);
    display_main();
}


void MainMenu::exec(int x) {
    (void) x;
    display_main();
}

void ToFile::exec(int x) {
    enter_fname(save_f);
    read_volts(x, init);
    uint8_t vs_hi = volts_size >> 8;
    uint8_t vs_lo = volts_size;
    uint8_t sr = current_samp_rate;
    data_file = fopen(file_name, "w");
    if (data_file == nullptr) {
        perror("Following error occured: ");
    } else {
        fputc(vs_hi, data_file);
        fputc(vs_lo, data_file);
        fputc(sr, data_file);
        fwrite(volts, sizeof(float), static_cast<size_t>(volts_size), data_file);
        fflush(data_file);
        fclose(data_file);
    }
    display_main();
}


void MeanAndStdDev::exec(int x) {
    //mean and standard deviation
    (void) x;
    float mean = 0.0;
    float var = 0.0;
    float stddev;
    clock_t begin = clock();
    for (int i = 0; i < volts_size; i++) {
        mean += volts[i];
    }
    mean /= volts_size;

    for (int i = 0; i < volts_size; i++) {
        var += pow((volts[i] - mean), 2.0);
    }
    var /= volts_size;
    stddev = sqrt(var);
    clock_t end = clock();
    auto elapsed_secs = double(end - begin);
    printf("Mean: %f\n", mean);
    printf("Standard deviation: %f\n", stddev);
    printf("Elapsed time(standard formula): %f\n", elapsed_secs);
    printf("\n");
    //mean and standard deviation with running stats
    float sum = 0.0;
    float sumsqrs = 0.0;
    mean = 0.0;
    begin = clock();
    for (int i = 0; i < volts_size; i++) {
        sum += volts[i];
        sumsqrs += pow(volts[i], 2.0);
        mean = sum / i;
        var = static_cast<float>((sumsqrs - (pow(sum, 2.0) / i)) / (i - 1));
        stddev = sqrt(var);
    }
    end = clock();
    elapsed_secs = double(end - begin);
    printf("Running stats mean: %f\n", mean);
    printf("Running stats standard deviation: %f\n", stddev);
    printf("Elapsed time(running stats): %f\n", elapsed_secs);
    printf("\n");
    display_main();
}

#ifndef DSP_TEST

void HistMeanAndStdDev::exec(int x) {
    (void) x;
    float mean = 0.0;
    float var;
    float stddev = 0.0;
    float sum = 0.0;
    float sumsqrs = 0.0;
    Plot plot;
    for (int i = 0; i < volts_size; i++) {
        sum += volts[i];
        sumsqrs += pow(volts[i], 2.0);
        mean = sum / i;
        var = static_cast<float>((sumsqrs - (pow(sum, 2.0) / i)) / (i - 1));
        stddev = sqrt(var);
    }
    printf("Running stats mean: %f\n", mean);
    printf("Running stats standard deviation: %f\n", stddev);
    plot.init_p();
    plot.histogram(volts, volts_size);
    plot.close_p();
    printf("\n");
    display_main();
}

void ConvertToWav::exec(int x) {
    (void) x;
    wav_h_gen h_gen;
    load_file(convert_open_f);
    enter_fname(convert_save_f);
    auto out_volts = new int16_t[volts_size];
    for (int i = 0; i < volts_size; ++i) {
        out_volts[i] = static_cast<int16_t>(volts[i] * 3276.7);
    }
    data_file = fopen(file_name, "w");
    if (data_file == nullptr) {
        perror("Following error occured: ");
    } else {
        wav_header *header = h_gen.get_wav_header(volts_size, current_samp_rate).get();
        fwrite(header->chunk_id, 4, 1, data_file);
        fwrite(&header->chunk_size, 4, 1, data_file);
        fwrite(header->chunk_format, 4, 1, data_file);
        fwrite(header->sub_chunk_1_id, 4, 1, data_file);
        fwrite(header->sub_chunk_1_size, 4, 1, data_file);
        fwrite(header->audio_format, 2, 1, data_file);
        fwrite(header->num_channels, 2, 1, data_file);
        fwrite(&header->sample_rate, 4, 1, data_file);
        fwrite(&header->byte_rate, 4, 1, data_file);
        fwrite(header->block_align, 2, 1, data_file);
        fwrite(header->bits_sample, 2, 1, data_file);
        fwrite(header->sub_chunk_2_id, 4, 1, data_file);
        fwrite(&header->sub_chunk_2_size, 4, 1, data_file);
        fwrite(out_volts, sizeof(int16_t), static_cast<size_t>(volts_size), data_file);
    //    fwrite(volts, sizeof(float), static_cast<size_t>(volts_size), data_file);
        fflush(data_file);
        fclose(data_file);
    }
    delete[] out_volts;
    back_to_main();
}

#endif

static inline void back_to_main() {
    printf("'0' back to main menu\n");
}

static inline void display_algos() {
    printf("'1' Mean and standard deviation\n");
    printf("'2' Histogram, mean and std deviation\n");
    printf("'0' back to main menu\n");
}

static inline void display_main() {
    printf("'1' analyze data from microphone\n");
    printf("'2' record a sample\n");
    printf("'3' load record & analyze\n");
    printf("'4' change sample rate\n");
    printf("'5' convert sample to .wav\n");
    printf("'6' exit\n");
}

static inline void read_volts(int x, Init &init) {
    switch (x) {
        case 512:
            adc_read_multiple(512, volts);
            break;
        case 1024:
        case 2048:
        case 4096:
        case 8192:
        case 12288:
            for (int i = 0; i < (x / 1024); i++) {
                adc_read_multiple(1024, &volts[i * 1024]);
            }
            break;
        default:
            printf("Illegal samples value. Exiting.\n");
            init.stop_adc(1);
    }
    volts_size = x;
}

static inline void enter_fname(const char *f_propmt) {
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
}

static inline void load_file(const char *f_propmt) {
    while (true) {
        enter_fname(f_propmt);
        data_file = fopen(file_name, "r");
        if (data_file != nullptr) {
            int vs_hi = fgetc(data_file);
            int vs_lo = fgetc(data_file);
            int sr = fgetc(data_file);
            volts_size = vs_hi << 8;
            volts_size |= vs_lo;
            current_samp_rate = sr;
            fread(volts, sizeof(float), static_cast<size_t>(volts_size), data_file);
            fflush(data_file);
            fclose(data_file);
            break;
        }
    }
}

