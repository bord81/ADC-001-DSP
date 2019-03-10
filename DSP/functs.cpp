#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "mem_man.h"
#include "funct.h"
#include "algos.h"
#include "services.h"

static const char *dat_ext = ".dat";
static const char *wav_ext = ".wav";
static const char *samp_dir = "samples/";
static const int PATH_LENGTH = 100;
static char smp_dir_path_dat[PATH_LENGTH] = {0};
static char smp_dir_path_wav[PATH_LENGTH] = {0};
static const char *load_f = "Enter filename to load (w/o extension, max 20 chars): \n";
static const char *save_f = "Enter filename to save (w/o extension, max 20 chars): \n";
static const char *cutoff_fr = "Enter cut-off frequency (in range from 0 to %d): \n";
static const char *cutoff_fr_lo = "Enter lower cut-off frequency (in range from 0 to %d): \n";
static const char *cutoff_fr_hi = "Enter upper cut-off frequency (in range up to %d): \n";
static const char *window_func = "Select window function (1 - Blackman, 2 - Hanning, 3 - Hamming)\n";
static int duration;
static mem_man *mm = new mem_man;
static services *srv = new services;

void MicInput::exec(int x) {
    (void) x;
    srv->pick_samp_dur();
}

void LoadFile::exec(int x) {
    srv->load_file(mm->get_slot(x), load_f);
    srv->display_main();
}

void ExitFromProg::exec(int x) {
    mm->release_mem();
    srv->stop_adc(x);
    exit(x);
}

void ChooseSampRate1::exec(int x) {
    if (x != 0) {
        duration = x;
    }
    srv->ch_samp_rate_1();
}

void ChooseSampRate2::exec(int x) {
    (void) x;
    srv->ch_samp_rate_2();
}

void SetSampRateAndRun::exec(int x) {
    int samples_count = (duration - 100) * srv->get_real_samp_rate(x);
    mm->get_slot(MEM_SLOT_1).sample_rate = x;
    srv->set_samp_rate(x);
    char *save_file = srv->enter_fname(save_f);
    srv->get_path(smp_dir_path_dat, PATH_LENGTH, samp_dir, save_file, dat_ext);
    srv->get_path(smp_dir_path_wav, PATH_LENGTH, samp_dir, save_file, wav_ext);
    srv->read_volts(samples_count, mm->get_slot(MEM_SLOT_1));
    srv->save_to_dat(mm->get_slot(MEM_SLOT_1), smp_dir_path_dat);
    srv->save_to_wav(mm->get_slot(MEM_SLOT_1), smp_dir_path_wav);
    srv->display_main();
}

void MainMenu::exec(int x) {
    (void) x;
    srv->display_main();
}

void DispAlgos::exec(int x) {
    (void) x;
    srv->display_algos();
}

void MeanAndStdDev::exec(int x) {
    //mean and standard deviation
    algos::mean_stddev(mm->get_slot(x));
    srv->display_main();
}

void HistMeanAndStdDev::exec(int x) {
    algos::mean_stddev_hist(mm->get_slot(x));
    srv->display_main();
}

void ConvInputSide::exec(int x) {
    (void) x;
    algos::conv_inp_side(mm->get_slot(MEM_SLOT_1), mm->get_slot(MEM_SLOT_2), mm->get_slot(MEM_SLOT_3));
    srv->display_main();
}

void ConvOutputSide::exec(int x) {
    (void) x;
    algos::conv_outp_side(mm->get_slot(MEM_SLOT_2), mm->get_slot(MEM_SLOT_1), mm->get_slot(MEM_SLOT_3));
    srv->display_main();
}

void DumpConv::exec(int x) {
    (void) x;
    char *save_file = srv->enter_fname(save_f);
    srv->get_path(smp_dir_path_dat, PATH_LENGTH, samp_dir, save_file, dat_ext);
    srv->get_path(smp_dir_path_wav, PATH_LENGTH, samp_dir, save_file, wav_ext);
    srv->save_to_dat(mm->get_slot(MEM_SLOT_3), smp_dir_path_dat);
    srv->save_to_wav(mm->get_slot(MEM_SLOT_3), smp_dir_path_wav);
    srv->display_main();
}

void Amplify::exec(int x) {
    algos::amplify(srv->choose_strength(), mm->get_slot(x), mm->get_slot(MEM_SLOT_3));
    srv->display_main();
}

void Attenuate::exec(int x) {
    algos::attenuate(srv->choose_strength(), mm->get_slot(x), mm->get_slot(MEM_SLOT_3));
    srv->display_main();
}

void LowPass::exec(int x) {
    double cr = srv->choose_cutoff_freq(cutoff_fr, mm->get_slot(x).sample_rate);
    int w_func = srv->choose_window_func(window_func);
    algos::low_pass(cr, w_func, mm->get_slot(x), mm->get_slot(MEM_SLOT_3),
                    mm->get_slot(MEM_SLOT_4), mm->get_filter(FILTER_SLOT_1));
    srv->display_main();
}

void HighPass::exec(int x) {
    double cr = srv->choose_cutoff_freq(cutoff_fr, mm->get_slot(x).sample_rate);
    int w_func = srv->choose_window_func(window_func);
    algos::high_pass(cr, w_func, mm->get_slot(x), mm->get_slot(MEM_SLOT_3),
                     mm->get_slot(MEM_SLOT_4), mm->get_filter(FILTER_SLOT_1));
    srv->display_main();
}

void BandPass::exec(int x) {
    double cr_lo = srv->choose_cutoff_freq(cutoff_fr_lo, mm->get_slot(x).sample_rate);
    double cr_hi;
    for (;;) {
        cr_hi = srv->choose_cutoff_freq(cutoff_fr_hi, mm->get_slot(x).sample_rate);
        if (cr_hi > cr_lo) {
            break;
        }
    }
    int w_func = srv->choose_window_func(window_func);
    algos::band_pass(cr_lo, cr_hi, w_func, mm->get_slot(x), mm->get_slot(MEM_SLOT_3),
                     mm->get_slot(MEM_SLOT_4), mm->get_filter(FILTER_SLOT_1));
    srv->display_main();
}

void BandStop::exec(int x) {
    double cr_lo = srv->choose_cutoff_freq(cutoff_fr_lo, mm->get_slot(x).sample_rate);
    double cr_hi;
    for (;;) {
        cr_hi = srv->choose_cutoff_freq(cutoff_fr_hi, mm->get_slot(x).sample_rate);
        if (cr_hi > cr_lo) {
            break;
        }
    }
    int w_func = srv->choose_window_func(window_func);
    algos::band_stop(cr_lo, cr_hi, w_func, mm->get_slot(x), mm->get_slot(MEM_SLOT_3),
                     mm->get_slot(MEM_SLOT_4), mm->get_filter(FILTER_SLOT_1));
    srv->display_main();
}

void Echo::exec(int x) {
    algos::echo(srv->get_real_samp_rate(mm->get_slot(x).sample_rate) / 2, 0.5,
            mm->get_slot(x), mm->get_slot(MEM_SLOT_3));
    srv->display_main();
}