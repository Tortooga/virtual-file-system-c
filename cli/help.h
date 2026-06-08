#ifndef HELP_H
#define HELP_H

#include "commands.h"

#include <stddef.h>

#define HELP_HINT_FIELD_LENGTH 200


typedef struct 
{
    const CommandFunction func;
    const char *name;
    const char *synopsis;
    const char *description;
} HelpHint;

static const HelpHint HELP_HINTS_TABLE[] = 
{
    {
        CMD_CD,
        "Change Directory",
        "cd [TARGET_DIRECTORY_PATH]",
        "Changes the current directory"
    },

    {
        CMD_PWD,
        "Print Working Directory",
        "pwd",
        "Prints the path of the current working directory."
    },

    {
        CMD_LS,
        "List Sub-Entries",
        "ls [optional: TARGET_DIRECTORY_PATH]",
        "If TARGET_DIRECTORY_PATH is not supplied the command lists the sub-entries of the Current Working Directory.\n"
        "If TARGET_DIRECTORY_PATH is supplied the command lists the sub-entries of the directory located at TARGET_DIRECTORY_PATH."
    },

    {
        CMD_TREE,
        "Tree",
        "tree [optional: TARGET_DIRECTORY_PATH]",
        "If TARGET_DIRECTORY_PATH is not supplied the command lists the hierarchical structure of the entries under the current working directory.\n"
        "If TARGET_DIRECTORY_PATH is supplied the command lists the hierarchical structure of the entries under directory located at TARGET_DIRECTORY_PATH."
    },

    {
        CMD_TOUCH,
        "Touch",
        "touch [FILE_NAME]",
        "Creates a sub file of the current working directory with name FILE_NAME."
    },

    {
        CMD_MKDIR,
        "Make Directory",
        "mkdir [DIRECTORY_NAME]",
        "Creates a sub directory of the current working directory with name DIRECTORY_NAME."
    },

    {
        CMD_RM,
        "Remove",
        "rm [TARGET_FILE_PATH]",
        "Deletes the file located at TARGET_FILE_PATH."
    },

    {
        CMD_RMDIR,
        "Remove Directory",
        "rmdir [optional: -r] [TARGET_DIRECTORY_PATH]",
        "Removes the directory at TARGET_DIRECTORY_PATH.\n"
        "If the directory has sub entries then it will not be removed unless \"-r\" the recursion option is supplied."
    },

    {
        CMD_MV,
        "Move",
        "mv [SOURCE_PATH] [TARGET_PATH]",
        "Moves the entry at SOURCE_PATH to the directory at TARGET_PATH."
    },

    {
        CMD_RENAME,
        "Rename",
        "rename [TARGET_ENTRY_PATH] [NEW_NAME]",
        "Changes the name of the entry located at TARGET_ENTRY_PATH to NEW_NAME."
    },

    {
        CMD_FIND,
        "Find",
        "find [TARGET_NAME]",
        "Lists the paths of all the entries with the name TARGET_NAME."
    },

    {
        CMD_CAT,
        "Concatenate (Legacy)",
        "cat [TARGET_FILE_PATH]",
        "Prints all the contents of the file located at TARGET_FILE_PATH."
    },

    {
        CMD_READ,
        "Read",
        "read [TARGET_FILE_PATH] [OFFSET] [AMOUNT]",
        "Reads AMOUNT number of bytes from the file located at TARGET_FILE_PATH starting at the position OFFSET."
    },

    {
        CMD_APPEND,
        "Append",
        "append [TARGET_FILE_PATH]",
        "Starts a multiline prompt. Press CTRL + D to submit.\n"
        "Writes data submitted onto the end of the file located at TARGET_FILE_PATH."
    },

    {
        CMD_CLEAR,
        "Clear",
        "clear [TARGET_FILE_PATH]",
        "Deletes all the data stored in the file located at TARGET_FILE_PATH."
    },

    {
        CMD_TRUNC,
        "Truncate",
        "trunc [TARGET_FILE_PATH] [AMOUNT]",
        "Removes AMOUNT number of bytes from the end of the file located at TARGET_FILE_PATH.\n"
        "*Note: As of now, Truncate removes storage bytes not logical bytes which means the empty bytes allocated at the end of a file count."
    },

    {CMD_HELP,
    "help",
    "help [COMMAND_FUNCTION]",
    "(;"},
};

static const size_t HELP_HINTS_TABLE_LENGTH = 
    sizeof(HELP_HINTS_TABLE) /
    sizeof(HELP_HINTS_TABLE[0]);

StatusCode print_str_command_function_help_hint(char *command_function_string);
#endif