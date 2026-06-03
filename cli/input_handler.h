#ifndef INPUT_HANDLER_H

#include "status.h"
#include "stddef.h"

#define INPUT_HANDLER_H

#define COMMAND_BUFFER_SIZE 256

#define MAX_MULTI_LINE_INPUT_LINES_AMOUNT 16
#define MAX_MULTI_LINE_INPUT_LINE_LENGTH 64 

#define INPUT_DATA_BUFFER_SIZE MAX_MULTI_LINE_INPUT_LINE_LENGTH * MAX_MULTI_LINE_INPUT_LINES_AMOUNT

typedef enum
{
    MULTI_LINE,
    NEW_LINE_TERMINATED
} InputMode;

typedef struct
{
    InputMode input_mode;
    char command_buffer[COMMAND_BUFFER_SIZE];

    char data_buffer[INPUT_DATA_BUFFER_SIZE];
} InputHandler;

//Data over buffer_size - 1 causes buffer over flow error
StatusCode new_line_terminated_read(char *buffer, size_t buffer_size);

//Terminates after EOF is encountered
StatusCode multi_line_read(char *buffer, size_t buffer_size, size_t *out_amount_read);
#endif