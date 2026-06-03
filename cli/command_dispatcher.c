#include "status.h"

#include "command_dispatcher.h"
#include "commands.h"
#include "input_handler.h"

#include "workspace.h"

#include <string.h>

//Calls appropriate command executer 
//Is responsible for prompting secondary input
StatusCode command_dispatch(Workspace *workspace, Command *cmd, InputHandler *input_handler)
{
    if (!workspace || !cmd || !input_handler)
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
        i < CMD_PROMPTING_FUNCTIONS_TABLE_LENGTH;
        i++
    )
    {
        if (cmd->func == CMD_PROMPTING_FUNCTIONS_TABLE[i].command_function)
        {
            StatusCode status = multi_line_read(input_handler->data_buffer, DATA_BUFFER_SIZE);
            
            if (status != SUCCESS)
            {
                return status;
            }

            return CMD_PROMPTING_FUNCTIONS_TABLE[i].executor_function(
                workspace, 
                cmd, 
                input_handler->data_buffer, 
                strlen(input_handler->data_buffer)
            );
        }
    }

    return COMMAND_FUNCTION_NOT_FOUND;
}