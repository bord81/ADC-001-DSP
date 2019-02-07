#ifndef DSP_H
#define DSP_H

#include <stdint.h>
#include <cstdlib>
#include <cstring>

#include "init.h"
#include "table.h"

extern "C" {
#include "spidriver_host.h"
#include "adcdriver_host.h"
}

void execute_State_Machine(FunctPtr fp);

#endif