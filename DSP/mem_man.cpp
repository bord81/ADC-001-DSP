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
    } else {
        return def_slot;
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