#include "input_handler.h"
#include "status.h"

#include <stddef.h>

StatusCode clear_input_buffer(InputHandler *in_handler)
{
    if (!in_handler)
    {
        return NULL_POINTER_PASSED;
    }

    for (size_t i = 0; i < INPUT_BUFFER_SIZE; i++)
    {
        in_handler->buffer[i] = '\0';
    }

    return SUCCESS; 
}