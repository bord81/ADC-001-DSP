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

const int MEM_VALID = 401;
const int MEM_INVALID = 402;

struct MemoryData {
    float *volts;
    int valid;
    int allocated;
    size_t volts_size;
    int sample_rate;
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

    void release_mem();

    static bool mem_is_ok(MemoryData &slot);

    static bool mem_alloc(MemoryData& slot, size_t size);

private:
    MemoryData slot_1;
    MemoryData slot_2;
    MemoryData slot_3;
    MemoryData def_slot;
};


#endif //ADC_001_DSP_MEM_MAN_H
