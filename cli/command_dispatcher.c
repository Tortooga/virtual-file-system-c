#include "status.h"

#include "command_dispatcher.h"
#include "commands.h"
#include "input_handler.h"

#include "workspace.h"

#include <string.h>

bool is_multi_line_read_prompting(CommandFunction command_func);
bool is_data_outputting(CommandFunction command_func);

//Calls appropriate command executer 
StatusCode command_dispatch(
    Workspace *workspace, 
    Command *cmd, 
    InputHandler *input_handler, 
    char *output_buffer, 
    size_t output_buffer_size,
    bool *exit_flag
)
{
    if (!workspace || !cmd || !input_handler || !output_buffer)
    {
        return NULL_POINTER_PASSED;
    }

    for (
        size_t i = 0;
        i < CMD_ARGUMENT_BASED_FUNCTIONS_TABLE_LENGTH;
        i++
    )
    {
        if (cmd->func == CMD_ARGUMENT_BASED_FUNCTIONS_TABLE[i].command_function)
        {
            return CMD_ARGUMENT_BASED_FUNCTIONS_TABLE[i].executor_function(workspace, cmd);
        }
    }

    for (
        size_t i = 0;
        i < CMD_PAYLOAD_BASED_FUNCTIONS_TABLE_LENGTH;
        i++
    )
    {
        if (cmd->func == CMD_PAYLOAD_BASED_FUNCTIONS_TABLE[i].command_function)
        {
            if (is_multi_line_read_prompting(cmd->func))
            {
                return CMD_PAYLOAD_BASED_FUNCTIONS_TABLE[i].executor_function(
                    workspace,
                    cmd,
                    input_handler->data_buffer,
                    INPUT_DATA_BUFFER_SIZE
                );
            }

            if (is_data_outputting(cmd->func))
            {
                return CMD_PAYLOAD_BASED_FUNCTIONS_TABLE[i].executor_function(
                    workspace,
                    cmd,
                    output_buffer,
                    output_buffer_size
                );
            }
            break;
        }
    }

    return COMMAND_FUNCTION_NOT_FOUND;
}

bool is_data_outputting(CommandFunction command_func)
{
    for (
        size_t i = 0;
        i < DATA_OUTPUTING_FUNCTIONS_LENGTH;
        i++
    )
    {
        if (command_func == DATA_OUTPUTING_FUNCTIONS[i])
        {
            return true;
        }
    }

    return false;
}

bool is_multi_line_read_prompting(CommandFunction command_func)
{
    for (
        size_t i = 0;
        i < MULTI_LINE_READ_PROMPTING_FUNCTIONS_LENGTH; 
        i++
    )
    {
        if (command_func == MULTI_LINE_READ_PROMPTING_FUNCTIONS[i])
        {
            return true;
        }
    }

    return false;
}