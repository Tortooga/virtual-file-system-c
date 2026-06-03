#include "input_handler.h"
#include "status.h"
#include "commands.h"

#include <stddef.h>
#include <unistd.h>
#include <string.h>

//Data over INPUT_BUFFER_SIZE - 1 causes buffer over flow error
StatusCode new_line_terminated_read(char *buffer, size_t buffer_length)
{
    if (!buffer)
    {
        return NULL_POINTER_PASSED;
    }

    if (buffer_length == 0)
    {
        return INSUFFICIENT_ARRAY_PASSED;
    }

    ssize_t amount_read = read(
        STDIN_FILENO,
        buffer,
        buffer_length
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
    if (amount_read == buffer_length)
    {
        return INPUT_BUFFER_OVER_FLOW;
    }

    if (buffer[amount_read - 1] == '\n')
    {
        buffer[amount_read - 1] = '\0';
    }
    else
    {
        //amount_read <= INPUT_BUFFER_SIZE
        buffer[amount_read] = '\0';
    }

    return SUCCESS;
}

//Terminates after EOF is encountered
StatusCode multi_line_read(char *buffer, size_t buffer_size, size_t *out_data_amount)
{
    if (!buffer || !out_data_amount)
    {
        return NULL_POINTER_PASSED;
    }

    *out_data_amount = 0;

    //Extra position to detect overflow 
    char line_buffer[MAX_MULTI_LINE_INPUT_LINE_LENGTH + 1];
    ssize_t amount_read;

    size_t cur_input_buffer_pos = 0;

    for (size_t i = 0; i < MAX_MULTI_LINE_INPUT_LINES_AMOUNT; i++)
    {
        amount_read = read(
            STDIN_FILENO,
            line_buffer,
            //We include the +1 so that if it is written into, we know that the input has overflown the buffer 
            MAX_MULTI_LINE_INPUT_LINE_LENGTH + 1
        );

        if (amount_read < 0)
        {
            return READ_ERROR;
        }

        //EOF
        if (amount_read == 0)
        {
            //guaranteed to be < INPUT_BUFFER_SIZE
            buffer[cur_input_buffer_pos] = '\0';
            return SUCCESS;
        }

        //If the input overflows the logical limit of MAX_INPUT_LINE_LENGTH 
        if (amount_read == MAX_MULTI_LINE_INPUT_LINE_LENGTH + 1)
        {
            return INPUT_LINE_IS_TOO_LONG;
        }

        //extra space for null terminator
        if (amount_read + cur_input_buffer_pos > buffer_size - 1)
        {
            return INPUT_BUFFER_OVER_FLOW;
        }

        memcpy(
            buffer + cur_input_buffer_pos,
            line_buffer,
            amount_read
        );
        
        *out_data_amount += amount_read;
        cur_input_buffer_pos += amount_read;
    }

    return TOO_MANY_LINES;
}