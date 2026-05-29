#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#define INPUT_BUFFER_SIZE 100

#include "status.h"
typedef enum
{
    MULTI_LINE,
    NEW_LINE_TERMINATED
} InputMode;

typedef struct
{
    InputMode input_mode;
    char buffer[INPUT_BUFFER_SIZE];

} InputHandler;

//Data over INPUT_BUFFER_SIZE - 1 causes buffer over flow error
StatusCode new_line_terminated_read(InputHandler *in_handler);

#endif