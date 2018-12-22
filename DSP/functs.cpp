#include <cmath>
#include <ctime>
#include "dsp.h"
#include "funct.h"

#ifdef DSP_TEST
#include <cstring>
#include "doctest.h"

extern int test_adc_samprate;

#endif

static float volts[12288];
static int volts_size;
static char file_name[21];
static FILE *data_file;

static const char *load_f = "Enter filename to load (max 20 chars): \n";
static const char *save_f = "Enter filename to save (max 20 chars): \n";

static inline void display_algos();

static inline void display_main();

static inline void read_volts(int x, Init & init);

static inline void enter_fname(const char *fname, FILE *h_stream);

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
    enter_fname(save_f, h_stream);
    data_file = fopen(file_name, "r");
    int s_hi = fgetc(data_file);
    int s_lo = fgetc(data_file);
    volts_size = s_hi << 8;
    volts_size |= s_lo;
    fread(volts, sizeof(float), volts_size, data_file);
    fflush(data_file);
    fclose(data_file);
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
    adc_set_samplerate(x);
    display_main();
}


void MainMenu::exec(int x) {
    (void) x;
    display_main();
}

void ToFile::exec(int x) {
    enter_fname(save_f, h_stream);
    read_volts(x, init);
    int s_hi = volts_size >> 8;
    int s_lo = volts_size;
    data_file = fopen(file_name, "w");
    fputc(s_hi, data_file);
    fputc(s_lo, data_file);
    fwrite(volts, sizeof(float), volts_size, data_file);
    fflush(data_file);
    fclose(data_file);
    display_main();
}


void MeanAndStdDev::exec(int x) {
    //mean and standard deviation
    float mean = 0.0;
    float var = 0.0;
    float stddev = 0.0;
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
    double elapsed_secs = double(end - begin);
    printf("Mean: %f\n", mean);
    printf("Standard deviation: %f\n", stddev);
    printf("Elapsed time(standard formula): %f", elapsed_secs);
    //mean and standard deviation with running stats
    float sum = 0.0;
    float sumsqrs = 0.0;
    mean = 0.0;
    var = 0.0;
    begin = clock();
    for (int i = 0; i < volts_size; i++) {
        sum += volts[i];
        sumsqrs += pow(volts[i], 2.0);
        mean = sum / i;
        var = (sumsqrs - (pow(sum, 2.0) / i)) / (i - 1);
        stddev = sqrt(var);
        end = clock();
        elapsed_secs = double(end - begin);
        printf("%d: mean: %f\n", i, mean);
        printf("%d: standard deviation: %f\n", i, stddev);
        printf("Elapsed time(running stats): %f", elapsed_secs);
    }
    display_main();
}


/* TODO: Implement HistMeanAndStdDev
class HistMeanAndStdDev : public Funct 
{
public:
  virtual void exec(int x) {  }
}; */

static inline void display_algos() {
    printf("'1' Mean and standard deviation\n");
    printf("'0' back to main menu\n");
}

static inline void display_main() {
    printf("'1' analyze data from microphone\n");
    printf("'2' record a sample\n");
    printf("'3' load record & analyze\n");
    printf("'4' change sample rate\n");
    printf("'5' exit\n");
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

static inline void enter_fname(const char *fname, FILE* h_stream) {
    memset(file_name, 0, sizeof(file_name));
    for (;;) {
        printf(fname);
        fgets(file_name, 20, h_stream);
        if (strlen(file_name) > 0) {
            break;
        }
    }
}

#ifdef DSP_TEST

static SetSampRate setSampRate;
static FILE* mem_file;
static ToFile to_file(mem_file);
static LoadFile load_file(mem_file);
static char* test_f = "test.dat\n";
static float volts_test[12288];

TEST_CASE("DSP test. SetSampRate") {
  setSampRate.exec(SAMP_RATE_31250);
  CHECK(test_adc_samprate == SAMP_RATE_31250);
  setSampRate.exec(SAMP_RATE_15625);
  CHECK(test_adc_samprate == SAMP_RATE_15625);
  setSampRate.exec(SAMP_RATE_10417);
  CHECK(test_adc_samprate == SAMP_RATE_10417);
}

TEST_CASE("DSP test. Record and load file") {
  bool test_ok = true;
  mem_file = fmemopen(test_f, strlen(test_f), "r");
  to_file.exec(512);
  memcpy(volts_test, volts, 512);
  load_file.exec(0);
  for (int i = 0; i < 512; i++) {
    if (volts_test[i] != volts[i]) {
      test_ok = false;
    }
  }
  CHECK(test_ok == true);
  to_file.exec(2048);
  memcpy(volts_test, volts, 2048);
  load_file.exec(0);
  for (int i = 0; i < 2048; i++) {
    if (volts_test[i] != volts[i]) {
      test_ok = false;
    }
  }
  CHECK(test_ok == true);
  to_file.exec(12288);
  memcpy(volts_test, volts, 12288);
  load_file.exec(0);
  for (int i = 0; i < 12288; i++) {
    if (volts_test[i] != volts[i]) {
      test_ok = false;
    }
  }
  CHECK(test_ok == true);
  fclose(mem_file);
}

#endif