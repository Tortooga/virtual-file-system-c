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
#define MAX_TOKEN_LENGTH MAX_CMD_ARGUMENT_LENGTH

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
    EXIT
} CommandFunction;


typedef struct 
{
    CommandFunction func;
    char opts[MAX_CMD_OPTION_AMOUNT];
    char arguments[MAX_CMD_ARGUMENT_AMOUNT][MAX_CMD_ARGUMENT_LENGTH];
} Command;


//Tokenizes command_str by replacing each ' ' with '\0' and storing pointers to the start of each token in out_tokens
//Consecutive occurences of COMMAND_DELIMITER are treated as one seperation
StatusCode cmd_tokenize(
    char *command_str,
    char **out_tokens,
    const size_t tokens_capacity,
    size_t *out_tokens_length
);


#endif