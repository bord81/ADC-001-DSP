#ifdef DSP_TEST
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#else
#include <unistd.h>
#include "dsp.h"
#include "funct.h"

int main(int argc, char *argv[]) {
	Init init_main;
	MainMenu mm;
	if (getuid() != 0) {
		printf("You must run this program as root. Exiting.\n");
		init_main.stop_adc(1);
	}
	init_main.start_adc();
	execute_State_Machine(&mm);
	return 0;
}
#endif
