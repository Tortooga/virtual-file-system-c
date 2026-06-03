#ifndef COMMAND_DISPATCHER_H
#define COMMAND_DISPATCHER_H

#include "status.h"

#include "commands.h"
#include "command_exec.h"
#include "input_handler.h"

#include "workspace.h"

#define PROMPTED_DATA_BUFFER_SIZE 256

typedef struct
{
    StatusCode (*executor_function)(Workspace *, Command *);
    CommandFunction command_function;
} CMDArgumentBasedFunctionEntry;

typedef struct
{
    StatusCode (*executor_function)(Workspace *, Command *, char *, size_t);
    CommandFunction command_function;
} CMDPromptingFunctionEntry;

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
    {cmd_rename_exec, CMD_RENAME}
};

static const CMDPromptingFunctionEntry CMD_PROMPTING_FUNCTIONS_TABLE[] =
{
    {cmd_append_exec, CMD_APPEND}
};

static const size_t CMD_ARGUMENT_BASED_FUNCTIONS_TABLE_LENGTH = 
    sizeof(CMD_ARGUMENT_BASED_FUNCTIONS_TABLE) /
    sizeof(CMD_ARGUMENT_BASED_FUNCTIONS_TABLE[0]);

static const size_t CMD_PROMPTING_FUNCTIONS_TABLE_LENGTH = 
    sizeof(CMD_PROMPTING_FUNCTIONS_TABLE) /
    sizeof(CMD_PROMPTING_FUNCTIONS_TABLE[0]);


//Calls appropriate command executer 
//Is responsible for prompting secondary input
StatusCode command_dispatch(Workspace *workspace, Command *cmd, InputHandler *input_handler);
#endif