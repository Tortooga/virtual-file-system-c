#include "command_exec.h"
#include "commands.h"
#include "input_handler.h"

#include "workspace.h"
#include "navigation.h"
#include "vfs_ops.h"
#include "file_io.h"

#include "path_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define NUMBER_SYSTEM_BASE 10

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

    if (nodes_amount == 0)
    {
        return SUCCESS;
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

StatusCode cmd_append_exec(Workspace *workspace, Command *cmd, char *data_buffer, size_t data_buffer_length)
{
    if (!workspace || !cmd || !data_buffer)
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

    if (data_buffer_length == 0)
    {
        return SUCCESS;
    }
    
    size_t amount_read;

    StatusCode status = multi_line_read(
        data_buffer,
        data_buffer_length,
        &amount_read
    );

    if (status != SUCCESS)
    {
        return status;
    }

    if (amount_read == 0)
    {
        return SUCCESS;
    }

    return ws_file_append(
        workspace,
        cmd->args[0],
        data_buffer,
        amount_read
    );
}

StatusCode cmd_cat_exec(Workspace *workspace, Command *cmd, char *data_buffer, size_t data_buffer_length)
{
    if (!workspace || !cmd || !data_buffer_length)
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

    size_t data_amount;

    StatusCode status = ws_file_read_all(
        workspace, 
        cmd->args[0],
        data_buffer,
        data_buffer_length,
        &data_amount
    );

    if (status != SUCCESS)
    {
        return status;
    }

    if (data_amount == 0)
    {
        return SUCCESS;
    }
    
    for (size_t i = 0; i < data_amount; i++)
    {
        printf("%c", data_buffer[i]);
    }

    printf("\n");

    return SUCCESS;
}

StatusCode cmd_clear_exec(Workspace *workspace, Command *cmd)
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

    return ws_file_clear(
        workspace,
        cmd->args[0]
    );
}

StatusCode cmd_read_exec(Workspace *workspace, Command *cmd, char *data_buffer, size_t data_buffer_length)
{
    if (!workspace || !cmd || !data_buffer)
    {
        return NULL_POINTER_PASSED;
    }

    if (cmd->opts_amount > 0)
    {
        return CMD_TOO_MANY_OPTS;
    }

    if (cmd->args_amount > 3)
    {
        return CMD_TOO_MANY_ARGS;
    }

    if (cmd->args_amount < 3)
    {
        return CMD_TOO_FEW_ARGS;
    }

    errno = 0;
    char *termination_point;
    long offset = strtol(
        cmd->args[1],
        &termination_point,
        NUMBER_SYSTEM_BASE
    );

    if (*termination_point != '\0')
    {
        return CMD_EXPECTED_INT;
    }

    if (errno == ERANGE)
    {
        return CMD_INPUT_OUT_OF_RANGE;
    }

    long amount = strtol(
        cmd->args[2],
        &termination_point,
        NUMBER_SYSTEM_BASE
    );

    if (*termination_point != '\0')
    {
        printf("%s", termination_point);
        return CMD_EXPECTED_INT;
    }

    if (errno == ERANGE)
    {
        return CMD_INPUT_OUT_OF_RANGE;
    }

    if (offset < 0 || amount < 0)
    {
        return CMD_EXPECTED_POSATIVE_NUMBER;
    }

    if (amount == 0)
    {
        return SUCCESS;
    }

    StatusCode status = ws_file_read_at(
        workspace,
        cmd->args[0],
        (size_t) offset,
        (size_t) amount,
        data_buffer,
        data_buffer_length
    );

    if (status != SUCCESS)
    {
        return status;
    }

    for (size_t i = 0; i < amount; i++)
    {
        printf("%c", data_buffer[i]);
    }

    printf("\n");
    return SUCCESS;
}

StatusCode cmd_trunc_exec(Workspace *workspace, Command *cmd)
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

    errno = 0;
    char *termination_point;
    
    long amount = strtol(
        cmd->args[1],
        &termination_point,
        NUMBER_SYSTEM_BASE
    );

    if (errno == ERANGE)
    {
        return CMD_INPUT_OUT_OF_RANGE;
    }

    if (*termination_point != '\0')
    {
        return CMD_EXPECTED_INT;
    }

    if (amount < 0)
    {
        return CMD_EXPECTED_POSATIVE_NUMBER;
    }

    return ws_file_truncate(
        workspace,
        cmd->args[0],
        amount
    );
}

StatusCode cmd_find_exec(Workspace *workspace, Command *cmd)
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

    //VFSNode only stores a pointer to the file/folder struct
    VFSNode node_buffer[QUERIES_NODE_BUFFER_SIZE];
    size_t nodes_amount;

    StatusCode status = ws_global_search_by_name(
        workspace,
        cmd->args[0],
        node_buffer,
        QUERIES_NODE_BUFFER_SIZE,
        &nodes_amount
    );

    if (status != SUCCESS)
    {
        return status;
    }

    if (nodes_amount == 0)
    {
        return SUCCESS;
    }

    char path_buffer[MAX_PATH_NODES_AMOUNT * (MAX_NAME_LENGTH + 1) + 1];
    for (size_t i = 0; i < nodes_amount; i++)
    {
        status = get_node_path(
            &node_buffer[i], 
            path_buffer, 
            MAX_PATH_NODES_AMOUNT * (MAX_NAME_LENGTH + 1) + 1
        );

        if (status != SUCCESS)
        {
            return status;
        }

        printf("%s\n", path_buffer);
    }

    return SUCCESS;
}

#include "folders.h"

StatusCode cmd_tree_exec(Workspace *workspace, Command *cmd)
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

    Folder *target_folder;

    if (cmd->args_amount == 0)
    {
        target_folder = workspace->cur_folder;
    }
    else
    {
        VFSNode node;
        StatusCode status = ws_resolve_path(
            workspace,
            cmd->args[0],
            &node
        );

        if (status != SUCCESS)
        {
            return status;
        }

        if (node.type != FOLDER_NODE)
        {
            return EXPECTED_FOLDER_GOT_FILE;
        }

        target_folder = node.node.folder;
    }

    return print_folder(target_folder);
}

