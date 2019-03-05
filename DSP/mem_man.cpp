#include <cstdlib>
#include <cstdio>

#include "mem_man.h"

MemoryData &mem_man::get_slot(int slot) {
    if (slot == MEM_SLOT_1) {
        return slot_1;
    } else if (slot == MEM_SLOT_2) {
        return slot_2;
    } else if (slot == MEM_SLOT_3) {
        return slot_3;
    } else if (slot == MEM_SLOT_4) {
        return slot_4;
    } else {
        return def_slot;
    }
}

MemoryFilter &mem_man::get_filter(int slot) {
    if (slot == FILTER_SLOT_1) {
        return f_slot_1;
    }
}

void mem_man::release_mem() {
    if (slot_1.allocated == MEM_ALLOC) {
        free(slot_1.volts);
        slot_1.allocated = MEM_FREE;
    }
    if (slot_2.allocated == MEM_ALLOC) {
        free(slot_2.volts);
        slot_2.allocated = MEM_FREE;
    }
    if (slot_3.allocated == MEM_ALLOC) {
        free(slot_3.volts);
        slot_3.allocated = MEM_FREE;
    }
    if (f_slot_1.alloc_taps == MEM_ALLOC) {
        free(f_slot_1.taps);
        f_slot_1.alloc_taps = MEM_FREE;
    }
}

bool mem_man::mem_is_ok(MemoryData &slot) {
    return slot.valid == MEM_VALID;
}

bool mem_man::mem_alloc(MemoryData &slot, size_t size) {
    if (!mem_is_ok(slot)) {
        printf("%s bad memory slot\n", __func__);
        return false;
    }
    if (slot.allocated == MEM_ALLOC) {
        free(slot.volts);
        slot.allocated = MEM_FREE;
    }
    slot.volts = static_cast<float *>(calloc(size, sizeof(float)));
    if (slot.volts != nullptr) {
        slot.allocated = MEM_ALLOC;
        slot.volts_size = size;
        return true;
    } else {
        printf("%s free memory mot available\n", __func__);
        return false;
    }
}

bool mem_man::mem_alloc_taps(MemoryFilter &slot, int size) {
    if (slot.alloc_taps == MEM_ALLOC) {
        free(slot.taps);
        slot.alloc_taps = MEM_FREE;
    }
    slot.taps = static_cast<double *>(calloc(size, sizeof(double)));
    if (slot.taps != nullptr) {
        slot.alloc_taps = MEM_ALLOC;
        slot.taps_size = size;
        return true;
    } else {
        printf("%s free memory mot available\n", __func__);
        return false;
    }
}

MemoryData &mem_man::mem_filter_to_data(MemoryFilter &in_slot, MemoryData &out_slot) {
    if (mem_alloc(out_slot, in_slot.taps_size)) {
        for (int i = 0; i < in_slot.taps_size; ++i) {
            out_slot.volts[i] = static_cast<float>(in_slot.taps[i]);
        }
    }
    return out_slot;
}