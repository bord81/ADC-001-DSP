#ifndef DSP_H
#define DSP_H

#include <stdint.h>
#include <cstdlib>
#include <cstring>

#include "init.h"
#include "table.h"

#ifndef DSP_TEST
extern "C" {
	#include "spidriver_host.h"
	#include "adcdriver_host.h"
}
#else
#include "adcdriver_host_mock.h"
#endif

void execute_State_Machine(FunctPtr fp);

#endif