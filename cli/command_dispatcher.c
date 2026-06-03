#include "status.h"

#include "command_dispatcher.h"
#include "commands.h"
#include "input_handler.h"

#include "workspace.h"


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
            return CMD_ARGUMENT_BASED_FUNCTIONS_TABLE[i].dispatcher_function(workspace, cmd);
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
            
        }
    }

    return COMMAND_FUNCTION_NOT_FOUND;
}