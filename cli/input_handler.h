#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#define INPUT_BUFFER_SIZE 100

typedef enum
{
    MULTILINE,
    LINETERMINATED
} InputMode;

typedef struct
{
    InputMode input_mode;
    char buffer[INPUT_BUFFER_SIZE];

} InputHandler;


#endif