#include "status.h"

#include "command_dispatcher.h"
#include "commands.h"

#include "workspace.h"



StatusCode command_dispatch(Workspace *workspace, Command *cmd)
{
    if (!workspace || !cmd)
    {
        return NULL_POINTER_PASSED;
    }

    for (
        size_t i = 0;
        i < COMMAND_DISPATCHER_FUNCTIONS_TABLE_LENGTH;
        i++
    )
    {
        if (cmd->func == COMMAND_DISPATCHER_FUNCTIONS_TABLE[i].command_function)
        {
            return COMMAND_DISPATCHER_FUNCTIONS_TABLE[i].dispatcher_function(workspace, cmd);
        }
    }

    return COMMAND_FUNCTION_NOT_FOUND;
}