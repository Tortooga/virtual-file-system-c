#ifndef COMMAND_DISPATCHER_H
#define COMMAND_DISPATCHER_H

#include "status.h"

#include "commands.h"
#include "command_exec.h"
#include "input_handler.h"

#include "workspace.h"

typedef struct
{
    StatusCode (*executor_function)(Workspace *, Command *);
    CommandFunction command_function;
} CMDArgumentBasedFunctionEntry;

typedef struct
{
    StatusCode (*executor_function)(Workspace *, Command *, char *, size_t);
    CommandFunction command_function;
} CMDPayloadBasedFunctionEntry;

static const CMDArgumentBasedFunctionEntry CMD_ARGUMENT_BASED_FUNCTIONS_TABLE[] =
{
    {cmd_cd_exec, CMD_CD},
    {cmd_pwd_exec, CMD_PWD},
    {cmd_ls_exec, CMD_LS},
    {cmd_touch_exec, CMD_TOUCH},
    {cmd_mkdir_exec, CMD_MKDIR},
    {cmd_rm_exec, CMD_RM},
    {cmd_rmdir_exec, CMD_RMDIR},
    {cmd_mv_exec, CMD_MV},
    {cmd_rename_exec, CMD_RENAME},
    {cmd_clear_exec, CMD_CLEAR},
    {cmd_trunc_exec, CMD_TRUNC},
    {cmd_find_exec, CMD_FIND},
    {cmd_tree_exec, CMD_TREE},
    {cmd_help_exec, CMD_HELP},
};

static const size_t CMD_ARGUMENT_BASED_FUNCTIONS_TABLE_LENGTH = 
    sizeof(CMD_ARGUMENT_BASED_FUNCTIONS_TABLE) /
    sizeof(CMD_ARGUMENT_BASED_FUNCTIONS_TABLE[0]);

static const CMDPayloadBasedFunctionEntry CMD_PAYLOAD_BASED_FUNCTIONS_TABLE[] =
{
    {cmd_append_exec, CMD_APPEND},
    {cmd_cat_exec, CMD_CAT},
    {cmd_read_exec, CMD_READ}
};

static const size_t CMD_PAYLOAD_BASED_FUNCTIONS_TABLE_LENGTH = 
    sizeof(CMD_PAYLOAD_BASED_FUNCTIONS_TABLE) /
    sizeof(CMD_PAYLOAD_BASED_FUNCTIONS_TABLE[0]);

static const CommandFunction MULTI_LINE_READ_PROMPTING_FUNCTIONS[] = 
{
    CMD_APPEND
};

static const size_t MULTI_LINE_READ_PROMPTING_FUNCTIONS_LENGTH = 
    sizeof(MULTI_LINE_READ_PROMPTING_FUNCTIONS) /
    sizeof(MULTI_LINE_READ_PROMPTING_FUNCTIONS[0]);

static const CommandFunction DATA_OUTPUTING_FUNCTIONS[] =
{
    CMD_CAT,
    CMD_READ,
};

static const size_t DATA_OUTPUTING_FUNCTIONS_LENGTH =
    sizeof(DATA_OUTPUTING_FUNCTIONS) /
    sizeof(DATA_OUTPUTING_FUNCTIONS[0]);

//Calls appropriate command executer 
//Is responsible for prompting secondary input
StatusCode command_dispatch(Workspace *workspace, Command *cmd, InputHandler *input_handler, char *output_buffer, size_t output_buffer_size);
#endif