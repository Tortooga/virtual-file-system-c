#include "command_exec.h"
#include "commands.h"

#include "workspace.h"
#include "navigation.h"
#include "vfs_ops.h"

#include "path_utils.h"

#include <stdio.h>

static void print_entry_name_column(char *name);

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

StatusCode cmd_ls_exec(Workspace *workspace, Command *cmd)
{
    if (!workspace || !cmd)
    {
        return NULL_POINTER_PASSED;
    }

    if (cmd->opts_amount > 0)
    {
        return CMD_TOO_MANY_OPTS;
    }

    if (cmd->args_amount > 1)
    {
        return CMD_TOO_MANY_OPTS;
    }

    VFSNode node_buffer[MAX_SUB_FILES_AMOUNT + MAX_SUB_FOLDERS_AMOUNT];
    size_t nodes_amount = 0;
    StatusCode status;
    if (cmd->args_amount == 1)
    {
        status = ws_get_sub_entries(
            workspace,
            cmd->args[0],
            node_buffer,
            MAX_SUB_FILES_AMOUNT + MAX_SUB_FOLDERS_AMOUNT,
            &nodes_amount
        );
    }
    else
    {
        status = ws_get_sub_entries(
            workspace,
            NULL,
            node_buffer,
            MAX_SUB_FILES_AMOUNT + MAX_SUB_FOLDERS_AMOUNT,
            &nodes_amount
        );
    }

    if (status != SUCCESS)
    {
        return status;
    }

    for (size_t i = 0; i < nodes_amount; i++)
    {
        if (i != 0 && i % SUB_ENTRIES_PRINTED_PER_LINE == 0)
        {
            printf("\n");
        }

        if (node_buffer[i].type == FOLDER_NODE)
        {
            print_entry_name_column(node_buffer[i].node.folder->name);
            continue;
        }

        print_entry_name_column(node_buffer[i].node.file->name);
    }

    printf("\n");

    return SUCCESS;
}

//caller guarantees name is not NULL
static void print_entry_name_column(char *name)
{
    printf(
        "%-*s",
        SUB_ENTRY_PADDING_WIDTH, 
        name
    );

    printf(
        "%*s",
        SUB_ENTRY_SPACING_WIDTH,
        ""
    );
}

StatusCode cmd_touch_exec(Workspace *workspace, Command *cmd)
{
    if (!workspace || !cmd)
    {
        return NULL_POINTER_PASSED;
    }

    if (cmd->opts_amount > 0)
    {
        return CMD_TOO_MANY_OPTS;
    }

    if (cmd->args_amount > 1)
    {
        return CMD_TOO_MANY_ARGS;
    }

    if (cmd->args_amount == 0)
    {
        return CMD_TOO_FEW_ARGS;
    }

    return ws_create_file(
        workspace,
        cmd->args[0]
    );
}

StatusCode cmd_mkdir_exec(Workspace *workspace, Command *cmd)
{
    if (!workspace || !cmd)
    {
        return NULL_POINTER_PASSED;
    }

    if (cmd->opts_amount > 0)
    {
        return CMD_TOO_MANY_OPTS;
    }

    if (cmd->args_amount > 1)
    {
        return CMD_TOO_MANY_ARGS;
    }

    if (cmd->args_amount == 0)
    {
        return CMD_TOO_FEW_ARGS;
    }

    return ws_create_folder(
        workspace,
        cmd->args[0]
    );
}

StatusCode cmd_rm_exec(Workspace *workspace, Command *cmd)
{
    if (!workspace || !cmd)
    {
        return NULL_POINTER_PASSED;
    }
    
    if (cmd->opts_amount > 0)
    {
        return CMD_TOO_MANY_OPTS;
    }   

    if (cmd->args_amount > 1)
    {
        return CMD_TOO_MANY_ARGS;
    }

    if (cmd->args_amount < 1)
    {
        return CMD_TOO_FEW_ARGS;
    }

    return ws_remove_file(
        workspace,
        cmd->args[0]
    );
}

StatusCode cmd_rmdir_exec(Workspace *workspace, Command *cmd)
{
    if (!workspace || !cmd)
    {
        return NULL_POINTER_PASSED;
    }

    if (cmd->opts_amount > 1)
    {
        return CMD_TOO_MANY_OPTS;
    }

    if (cmd->args_amount > 1)
    {
        return CMD_TOO_MANY_ARGS;
    }

    if (cmd->args_amount == 0)
    {
        return CMD_TOO_FEW_ARGS;
    }

    bool recursive_flag = false;

    if (cmd->opts_amount == 1)
    {
        if (cmd->opts[0] != 'r')
        {
            return CMD_UNKNOWN_OPT;
        }

        recursive_flag = true;
    }

    return ws_remove_folder(
        workspace,
        cmd->args[0],
        recursive_flag
    );
}

StatusCode cmd_mv_exec(Workspace *workspace, Command *cmd)
{
    if (!workspace || !cmd)
    {
        return NULL_POINTER_PASSED;
    }

    if (cmd->opts_amount > 0)
    {
        return CMD_TOO_MANY_OPTS;
    }

    if (cmd->args_amount > 2)
    {
        return CMD_TOO_MANY_ARGS;
    }

    if (cmd->args_amount < 2)
    {
        return CMD_TOO_FEW_ARGS;
    }

    return ws_move_node(
        workspace,
        cmd->args[0],
        cmd->args[1]
    );
}

StatusCode cmd_rename_exec(Workspace *workspace, Command *cmd)
{
    if (!workspace || !cmd)
    {
        return NULL_POINTER_PASSED;
    }

    if (cmd->opts_amount > 0)
    {
        return CMD_TOO_MANY_OPTS;
    }

    if (cmd->args_amount > 2)
    {
        return CMD_TOO_MANY_ARGS;
    }

    if (cmd->args_amount < 2)
    {
        return CMD_TOO_FEW_ARGS;
    }

    return ws_rename_node(
        workspace,
        cmd->args[0],
        cmd->args[1]
    );
}