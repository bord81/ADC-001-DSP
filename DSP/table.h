#ifndef TABLE_H
#define TABLE_H

#include "funct.h"

const int DURATION_1 = 101;
const int DURATION_2 = 102;
const int DURATION_5 = 105;
const int DURATION_10 = 110;

struct State_Entry {
    unsigned int current_state_id;
    unsigned char transition_letter;
    unsigned int next_state_id;
    FunctPtr action;
    unsigned int parameter;
};

State_Entry const *table_begin();

State_Entry const *table_end();

void execute_State_Machine(FunctPtr fp);

#endif // TABLE_H
