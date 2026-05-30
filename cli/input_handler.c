#include "input_handler.h"
#include "status.h"
#include "commands.h"

#include <stddef.h>
#include <unistd.h>
#include <string.h>

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

    /*StatusCode status = clear_input_buffer(in_handler);

    if (status != SUCCESS)
    {
        return status;
    }
    */
    ssize_t amount_read = read(
        STDIN_FILENO,
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

//Terminates after EOF is encountered
StatusCode multi_line_read(InputHandler *in_handler)
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

    //Extra position to detect overflow 
    char line_buffer[MAX_INPUT_LINE_LENGTH + 1];
    ssize_t amount_read;

    size_t cur_input_buffer_pos = 0;

    for (size_t i = 0; i < MAX_INPUT_LINES_AMOUNT; i++)
    {
        amount_read = read(
            STDIN_FILENO,
            line_buffer,
            //We include the +1 so that if it is written into, we know that the input has overflown the buffer 
            MAX_INPUT_LINE_LENGTH + 1
        );

        if (amount_read < 0)
        {
            return READ_ERROR;
        }

        //EOF
        if (amount_read == 0)
        {
            //guaranteed to be < INPUT_BUFFER_SIZE
            in_handler->buffer[cur_input_buffer_pos] = '\0';
            return SUCCESS;
        }

        //If the input overflows the logical limit of MAX_INPUT_LINE_LENGTH 
        if (amount_read == MAX_INPUT_LINE_LENGTH + 1)
        {
            return CMD_IS_TOO_LONG;
        }

        //extra space for null terminator
        if (amount_read + cur_input_buffer_pos > INPUT_BUFFER_SIZE - 1)
        {
            return INPUT_BUFFER_OVER_FLOW;
        }

        memcpy(
            in_handler->buffer + cur_input_buffer_pos,
            line_buffer,
            amount_read
        );
        
        cur_input_buffer_pos += amount_read;
    }

    return TOO_MANY_LINES;
}