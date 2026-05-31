#include "command_exec.h"
#include "commands.h"

#include "workspace.h"
#include "navigation.h"

StatusCode cmd_cd_exec(Workspace *workspace, Command *cmd)
{
    if (!workspace || !cmd)
    {
        return NULL_POINTER_PASSED;
    }

    if (cmd->args_amount == 0)
    {
        return CMD_TOO_FEW_ARGS;
    }

    if (cmd->args_amount > 1)
    {
        return CMD_TOO_MANY_ARGS;
    }

    if (cmd->opts_amount > 0)
    {
        return CMD_TOO_FEW_OPTS;
    }

    return ws_change_cur_folder(
        workspace,
        cmd->args[0]
    );
}

StatusCode cmd_pwd_exec(Workspace *workspace, Command *cmd)
{
    if (!workspace || !cmd)
    {
        return NULL_POINTER_PASSED;
    }

    printf("%s\n", workspace->cur_path);
    return SUCCESS;
}