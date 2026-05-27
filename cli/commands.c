#include "commands.h"
#include "status.h"

#include <stddef.h>
#include <stdbool.h>
#include <string.h>

//Tokenizes command_str by replacing each ' ' with '\0' and storing pointers to the start of each token in out_tokens
//Consecutive occurences of COMMAND_DELIMITER are treated as one seperation
StatusCode cmd_tokenize(
    char *command_str,
    char **out_tokens,
    const size_t tokens_capacity,
    size_t *out_tokens_length
)
{
    if (!command_str || !out_tokens || !out_tokens_length)
    {
        return NULL_POINTER_PASSED;
    }

    *out_tokens_length = 0;

    if (tokens_capacity == 0)
    {
        return INSUFFICIENT_ARRAY_PASSED;
    }
    
    size_t command_length = strlen(command_str);
    
    if (command_length == 0)
    {
        return CMD_IS_EMPTY;
    }

    size_t cur_char_index = 0;

    for (; cur_char_index < command_length; cur_char_index++)
    {
        if (command_str[cur_char_index] != COMMAND_DELIMITER)
        {
            break;
        }
    }

    if (cur_char_index == command_length)
    {
        return CMD_IS_EMPTY;
    }

    //Storing the address of the first token
    out_tokens[0] = &command_str[cur_char_index];
    *out_tokens_length = 1;

    //we start at 1 as the first token has already been stored
    size_t cur_token_index = 1;

    for (; cur_char_index < command_length; cur_char_index++)
    {
        if (command_str[cur_char_index] == COMMAND_DELIMITER)
        {
            //if space comes directly before null terminator we terminate
            if (cur_char_index + 1 >= command_length)
            {
                return SUCCESS;
            }

            if (command_str[cur_char_index + 1] == COMMAND_DELIMITER)
            {
                continue;
            }

            if (cur_token_index >= tokens_capacity)
            {
                return TOO_MANY_TOKENS;
            }

            //We store the address of the beginning of the next token
            out_tokens[cur_token_index] = &command_str[cur_char_index + 1];

            command_str[cur_char_index] = '\0';

            cur_token_index++;
            (*out_tokens_length)++;
        }
    }

    return SUCCESS;
}


//command_str must be a C string
StatusCode cmd_parse(
    char *command_str,
    Command *out_command
)
{
    if (!command_str || !out_command)
    {
        return NULL_POINTER_PASSED;
    }

    size_t command_str_length = strlen(command_str);
    
    size_t cur_char_index = 0;

    for (; cur_char_index < command_str_length; cur_char_index++)
    {
        
    }

    size_t cur_arg_index = 0;
    size_t cur_opt_index = 0;

    size_t cur_item_length;
    for (; cur_char_index < command_str_length; cur_char_index++)
    {
        if (cur_arg_index >= MAX_CMD_ARGUMENT_AMOUNT)
        {
            return TOO_MANY_ARGUMENTS;
        }

        if (cur_opt_index >= MAX_CMD_OPTION_AMOUNT)
        {
            return TOO_MANY_OPTIONS;
        }


    }

    return IMPLEMENTATION_INCOMPLETE;
}
