#include <cstdio>
#include <cstdint>

extern "C" {
#include "spidriver_host.h"
#include "adcdriver_host.h"
}

#include "table.h"
#include "services.h"

static MicInput mic_input;
static LoadFile load_file;
static ExitFromProg exit_from_prog;
static ChooseSampRate1 ch_samp_rate1;
static ChooseSampRate2 ch_samp_rate2;
static SetSampRateAndRun set_samp_rate_run;
static MainMenu main_menu;
static DispAlgos disp_algos;
static MeanAndStdDev mean_and_std_dev;
static HistMeanAndStdDev hist_mean_and_std_dev;
static ConvInputSide conv_inp_side;
static ConvOutputSide conv_out_side;
static DumpConv dump_conv;
static Amplify amplify;
static Attenuate attenuate;
static LowPass low_pass;
static HighPass high_pass;
static BandPass band_pass;
static BandStop band_stop;

/* Main state machine table */
static const State_Entry main_menu_entries[] =
        {
                {0, '1', 1, &mic_input,             0},
                {0, '2', 0, &load_file,             MEM_SLOT_1},
                {0, '3', 0, &load_file,             MEM_SLOT_2},
                {0, '4', 5, &disp_algos,            0},
                {0, '5', 6, &disp_algos,            0},
                {0, '6', 0, &conv_inp_side,         0},
                {0, '7', 0, &conv_out_side,         0},
                {0, '8', 0, &dump_conv,             0},
                {0, '9', 0, &exit_from_prog,        0},
                {1, '1', 2, &ch_samp_rate1,         DURATION_1},
                {1, '2', 2, &ch_samp_rate1,         DURATION_2},
                {1, '3', 2, &ch_samp_rate1,         DURATION_5},
                {1, '4', 2, &ch_samp_rate1,         DURATION_10},
                {1, '5', 0, &main_menu,             0},
                {2, '1', 0, &set_samp_rate_run, SAMP_RATE_31250},
                {2, '2', 0, &set_samp_rate_run, SAMP_RATE_15625},
                {2, '3', 0, &set_samp_rate_run, SAMP_RATE_10417},
                {2, '4', 0, &set_samp_rate_run, SAMP_RATE_5208},
                {2, '5', 0, &set_samp_rate_run, SAMP_RATE_2604},
                {2, '6', 0, &set_samp_rate_run, SAMP_RATE_1008},
                {2, '7', 0, &set_samp_rate_run, SAMP_RATE_504},
                {2, '8', 0, &set_samp_rate_run, SAMP_RATE_400P6},
                {2, '9', 0, &set_samp_rate_run, SAMP_RATE_200P3},
                {2, 'n', 3, &ch_samp_rate2,         0},
                {2, '0', 0, &main_menu,             0},
                {3, '1', 0, &set_samp_rate_run, SAMP_RATE_100P2},
                {3, '2', 0, &set_samp_rate_run, SAMP_RATE_59P98},
                {3, '3', 0, &set_samp_rate_run, SAMP_RATE_50},
                {3, 'p', 2, &ch_samp_rate1,         0},
                {3, '0', 0, &main_menu,             0},
                {5, '1', 0, &mean_and_std_dev,      MEM_SLOT_1},
                {5, '2', 0, &hist_mean_and_std_dev, MEM_SLOT_1},
                {5, '3', 0, &amplify,               MEM_SLOT_1},
                {5, '4', 0, &attenuate,             MEM_SLOT_1},
                {5, '5', 0, &low_pass,              MEM_SLOT_1},
                {5, '6', 0, &high_pass,             MEM_SLOT_1},
                {5, '7', 0, &band_pass,             MEM_SLOT_1},
                {5, '8', 0, &band_stop,             MEM_SLOT_1},
                {5, '0', 0, &main_menu,             0},
                {6, '1', 0, &mean_and_std_dev,      MEM_SLOT_2},
                {6, '2', 0, &hist_mean_and_std_dev, MEM_SLOT_2},
                {6, '3', 0, &amplify,               MEM_SLOT_2},
                {6, '4', 0, &attenuate,             MEM_SLOT_2},
                {6, '5', 0, &low_pass,              MEM_SLOT_2},
                {6, '6', 0, &high_pass,             MEM_SLOT_2},
                {6, '7', 0, &band_pass,             MEM_SLOT_2},
                {6, '8', 0, &band_stop,             MEM_SLOT_2},
                {6, '0', 0, &main_menu,             0},


        };

static const unsigned int TABLE_SIZE = sizeof(main_menu_entries) / sizeof(main_menu_entries[0]);

State_Entry const *table_begin() {
    return &main_menu_entries[0];
}

State_Entry const *table_end() {
    return &main_menu_entries[TABLE_SIZE];
}

/* State machine taking care of screen workflows */
void execute_State_Machine(FunctPtr fp) {
    unsigned int current_state = 0;
    int transition_letter = 0;
    fp->exec(0);
    while (true) {
        transition_letter = getchar();
        State_Entry const *p_entry = table_begin();
        State_Entry const *const p_table_end = table_end();
        bool state_found = false;
        while ((!state_found) && (p_entry != p_table_end)) {
            if (p_entry->current_state_id == current_state) {
                if ((transition_letter < 0xFF && transition_letter > 0) &&
                    (p_entry->transition_letter == (unsigned char) transition_letter)) {
                    p_entry->action->exec(p_entry->parameter);
                    current_state = p_entry->next_state_id;
                    state_found = true;
                    break;
                }
            }
            ++p_entry;
        }
    }
}