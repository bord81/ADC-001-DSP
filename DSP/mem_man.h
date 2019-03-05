//
// Memory definitions
//

#ifndef ADC_001_DSP_MEM_MAN_H
#define ADC_001_DSP_MEM_MAN_H

const int MEM_FREE = 200;
const int MEM_ALLOC = 201;

const int MEM_SLOT_1 = 301;
const int MEM_SLOT_2 = 302;
const int MEM_SLOT_3 = 303;
const int MEM_SLOT_4 = 304;
const int FILTER_SLOT_1 = 305;

const int MEM_VALID = 401;
const int MEM_INVALID = 402;

struct MemoryData {
    size_t volts_size;
    float *volts;
    int valid;
    int allocated;
    int sample_rate;
};

struct MemoryFilter {
    double *taps;
    int alloc_taps;
    int taps_size;
};


class mem_man {
public:
    mem_man() {
        slot_1.volts = nullptr;
        slot_1.volts_size = 0;
        slot_1.sample_rate = 0;
        slot_1.valid = MEM_VALID;
        slot_1.allocated = MEM_FREE;

        slot_2.volts = nullptr;
        slot_2.volts_size = 0;
        slot_2.sample_rate = 0;
        slot_2.valid = MEM_VALID;
        slot_2.allocated = MEM_FREE;

        slot_3.volts = nullptr;
        slot_3.volts_size = 0;
        slot_3.sample_rate = 0;
        slot_3.valid = MEM_VALID;
        slot_3.allocated = MEM_FREE;

        slot_4.volts = nullptr;
        slot_4.volts_size = 0;
        slot_4.sample_rate = 0;
        slot_4.valid = MEM_VALID;
        slot_4.allocated = MEM_FREE;

        f_slot_1.taps = nullptr;
        f_slot_1.taps_size = 0;
        f_slot_1.alloc_taps = MEM_FREE;

        def_slot.volts = nullptr;
        def_slot.volts_size = -1;
        def_slot.sample_rate = -1;
        def_slot.valid = MEM_INVALID;
        def_slot.allocated = MEM_FREE;
    }

    ~mem_man() {
        release_mem();
    }

    MemoryData &get_slot(int slot);

    MemoryFilter &get_filter(int slot);

    void release_mem();

    static bool mem_is_ok(MemoryData &slot);

    static bool mem_alloc(MemoryData &slot, size_t size);

    static bool mem_alloc_taps(MemoryFilter &slot, int size);

    static MemoryData & mem_filter_to_data(MemoryFilter &in_slot, MemoryData &out_slot);

private:
    //slot_1 and slot_2 - samples slots
    MemoryData slot_1;
    MemoryData slot_2;
    //convolution slot
    MemoryData slot_3;
    //filter slots
    MemoryData slot_4;
    MemoryFilter f_slot_1;
    //reserved slot
    MemoryData def_slot;
};


#endif //ADC_001_DSP_MEM_MAN_H
