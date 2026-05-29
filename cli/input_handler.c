#include "input_handler.h"
#include "status.h"

#include <stddef.h>
#include <unistd.h>

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

//Data over INPUT_BUFFER_SIZE - 1 causes buffer over flow error
StatusCode new_line_terminated_read(InputHandler *in_handler)
{
    if (!in_handler)
    {
        return NULL_POINTER_PASSED;
    }

    StatusCode status = clear_input_buffer(in_handler);

    if (status != SUCCESS)
    {
        return status;
    }

    ssize_t amount_read = read(
        0,
        in_handler->buffer,
        INPUT_BUFFER_SIZE
    );

    if (amount_read < 0)
    {
        return READ_ERROR;
    }

    if (amount_read == 0)
    {
        return ATTEMPTED_TO_READ_AT_EOF;
    }

    //The input buffer upper limit exposed to the user is INPUT_BUFFER_SIZE - 1
    if (amount_read == INPUT_BUFFER_SIZE)
    {
        return INPUT_BUFFER_OVER_FLOW;
    }

    if (in_handler->buffer[amount_read - 1] == '\n')
    {
        in_handler->buffer[amount_read - 1] = '\0';
    }
    else
    {
        //amount_read <= INPUT_BUFFER_SIZE
        in_handler->buffer[amount_read] = '\0';
    }

    return SUCCESS;
}