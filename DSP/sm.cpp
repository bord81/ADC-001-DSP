#include "table.h"

/* State machine taking care of screen workflows */
void execute_State_Machine(FunctPtr fp)
{
    unsigned int current_state = 0;
    int transition_letter = 0;
    fp->exec(0);
    while (1)
    {
        transition_letter = getchar();
        State_Entry const *  p_entry = table_begin();
        State_Entry const * const  p_table_end =  table_end();
        bool state_found = false;
        while ((!state_found) && (p_entry != p_table_end))
        {
            if (p_entry->current_state_id == current_state)
            {
                if ((transition_letter < 0xFF && transition_letter > 0) && (p_entry->transition_letter == (unsigned char)transition_letter))
                {
                    p_entry->action->exec(p_entry->parameter);
                    current_state = p_entry->next_state_id;
                    state_found = true;
                    break;
                }
             }
             ++p_entry;
         }
    }
}
