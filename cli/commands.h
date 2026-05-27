#ifndef COMMANDS_H
#define COMMANDS_H

#include "settings.h"
#include "status.h"

#include <stdbool.h>
#include <stddef.h>

#define MAX_CMD_ARGUMENT_LENGTH MAX_NAME_LENGTH * MAX_PATH_NODES_AMOUNT
#define MAX_CMD_ARGUMENT_AMOUNT 2
#define MAX_CMD_OPTION_AMOUNT 1

#define COMMAND_DELIMITER ' '
#define MAX_COMMAND_LENGTH MAX_CMD_ARGUMENT_AMOUNT * MAX_CMD_ARGUMENT_LENGTH + MAX_CMD_OPTION_AMOUNT
#define MAX_TOKEN_LENGTH MAX_CMD_ARGUMENT_LENGTH
#define MAX_TOKENS_AMOUNT 1 + MAX_CMD_ARGUMENT_AMOUNT + MAX_CMD_OPTION_AMOUNT

typedef enum {
    //Navigation
    CMD_CD,
    CMD_PWD,
    CMD_LS,
    CMD_TREE,

    //File System Operations
    CMD_TOUCH,
    CMD_MKDIR,
    CMD_RM,
    CMD_RMDIR,
    CMD_MV,
    CMD_RENAME,

    //queries
    CMD_FIND,

    //Space
    CMD_DF,
    CMD_DU,
    CMD_STAT,

    //file IO
    CMD_CAT,
    CMD_READAT,
    CMD_APPEND,
    CMD_CLEAR,

    //UI
    CMD_HELP,
    CMD_EXIT
} CommandFunction;


typedef struct 
{
    CommandFunction func;
    char opts[MAX_CMD_OPTION_AMOUNT];
    char arguments[MAX_CMD_ARGUMENT_AMOUNT][MAX_CMD_ARGUMENT_LENGTH];
} Command;

typedef struct
{
    CommandFunction func;
    char *func_str;
} CommandEntry;

static const CommandEntry COMMAND_TABLE[] =
{
    // Navigation
    {CMD_CD, "cd"},
    {CMD_PWD, "pwd"},
    {CMD_LS, "ls"},
    {CMD_TREE, "tree"},

    // File System Operations
    {CMD_TOUCH, "touch"},
    {CMD_MKDIR, "mkdir"},
    {CMD_RM, "rm"},
    {CMD_RMDIR, "rmdir"},
    {CMD_MV, "mv"},
    {CMD_RENAME, "rename"},

    // Queries
    {CMD_FIND, "find"},

    // Space
    {CMD_DF, "df"},
    {CMD_DU, "du"},
    {CMD_STAT, "stat"},

    // File IO
    {CMD_CAT, "cat"},
    {CMD_READAT, "readat"},
    {CMD_APPEND, "append"},
    {CMD_CLEAR, "clear"},

    // UI
    {CMD_HELP, "help"},
    {CMD_EXIT, "exit"}
};

static const size_t COMMAND_TABLE_LENGTH = sizeof(COMMAND_TABLE) / sizeof(COMMAND_TABLE[0]);
#endif