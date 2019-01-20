#ifndef TABLE_H
#define TABLE_H

#include "funct.h"

struct State_Entry {
    unsigned int current_state_id;
    unsigned char transition_letter;
    unsigned int next_state_id;
    FunctPtr action;
    unsigned int parameter;
};

State_Entry const *table_begin(void);

State_Entry const *table_end(void);

void execute_State_Machine(FunctPtr fp);

#endif // TABLE_H
