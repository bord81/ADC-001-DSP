#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include "table.h"
#include "services.h"

int main(int argc, char *argv[]) {
    if (getuid() != 0) {
        printf("You must run this program as root. Exiting.\n");
        exit(1);
    }
    services::start_adc();
    printf("\n");
    printf("===================================================\n");
    printf("Playing around with basics of DSP on BBB. Have fun!\n");
    printf("===================================================\n");
    printf("\n");
    MainMenu mm;
    execute_State_Machine(&mm);
    return 0;
}
