#ifndef DSP_TEST

#include <cstdio>

#include "dsp.h"

static MicInput mic_input;
static LoadFile load_file(stdin);
static ExitFromProg exit_from_prog;
static ToArrayAndRun to_array_and_run;
static ChooseSampRate1 ch_samp_rate1;
static ChooseSampRate2 ch_samp_rate2;
static SetSampRate set_samp_rate;
static MainMenu main_menu;
static ToFile to_file(stdin);
static MeanAndStdDev mean_and_std_dev;
//static HistMeanAndStdDev hist_mean_and_std_dev;

/* Main state machine table */
static const State_Entry main_menu_entries[] =
{
	{ 0, '1', 1, &mic_input, 0},
	{ 0, '2', 2, &mic_input, 0},
	{ 0, '3', 11, &load_file, 0},
	{ 0, '4', 3, &ch_samp_rate1, 0},
	{ 0, '5', 4, &exit_from_prog, 0},
	{ 1, '1', 11, &to_array_and_run, 512},
	{ 1, '2', 11, &to_array_and_run, 1024},
	{ 1, '3', 11, &to_array_and_run, 2048},
	{ 1, '4', 11, &to_array_and_run, 4096},
	{ 1, '5', 11, &to_array_and_run, 8192},
	{ 1, '6', 11, &to_array_and_run, 12288},
	{ 1, '7', 0, &main_menu, 0},
	{ 2, '1', 0, &to_file, 512},
	{ 2, '2', 0, &to_file, 1024},
	{ 2, '3', 0, &to_file, 2048},
	{ 2, '4', 0, &to_file, 4096},
	{ 2, '5', 0, &to_file, 8192},
	{ 2, '6', 0, &to_file, 12288},
	{ 2, '7', 0, &main_menu, 0},
	{ 11, '1', 0, &mean_and_std_dev, 0},
//	{ 11, '2', 0, &hist_mean_and_std_dev, 0},
//	{ 11, 'n', 11, &load_next_algos, 0},
	{ 11, '0', 0, &main_menu, 0},
	{ 3, '1', 0, &set_samp_rate, SAMP_RATE_31250},
	{ 3, '2', 0, &set_samp_rate, SAMP_RATE_15625},
	{ 3, '3', 0, &set_samp_rate, SAMP_RATE_10417},
	{ 3, '4', 0, &set_samp_rate, SAMP_RATE_5208},
	{ 3, '5', 0, &set_samp_rate, SAMP_RATE_2604},
	{ 3, '6', 0, &set_samp_rate, SAMP_RATE_1008},
	{ 3, '7', 0, &set_samp_rate, SAMP_RATE_504},
	{ 3, '8', 0, &set_samp_rate, SAMP_RATE_400P6},
	{ 3, '9', 0, &set_samp_rate, SAMP_RATE_200P3},
	{ 3, 'n', 5, &ch_samp_rate2, 0},
	{ 3, '0', 0, &main_menu, 0},
	{ 5, '1', 0, &set_samp_rate, SAMP_RATE_100P2},
	{ 5, '2', 0, &set_samp_rate, SAMP_RATE_59P98},
	{ 5, '3', 0, &set_samp_rate, SAMP_RATE_50},
	{ 5, 'p', 3, &ch_samp_rate1, 0},
	{ 5, '0', 0, &main_menu, 0},
};

static const unsigned int  TABLE_SIZE =  sizeof(main_menu_entries) / sizeof(main_menu_entries[0]);

State_Entry const * table_begin(void)
{
    return &main_menu_entries[0];
}

State_Entry const * table_end(void)
{
    return &main_menu_entries[TABLE_SIZE];
}  

#endif
