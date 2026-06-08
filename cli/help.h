#ifndef HELP_H
#define HELP_H

#include "commands.h"

#include <stddef.h>

#define HELP_HINT_FIELD_LENGTH 64


typedef struct 
{
    const CommandFunction func;
    const char name[HELP_HINT_FIELD_LENGTH];
    const char synopsis[HELP_HINT_FIELD_LENGTH];
    const char description[HELP_HINT_FIELD_LENGTH];
} HelpHint;

static const HelpHint HELP_HINTS_TABLE[] = 
{
    {CMD_CD,
    "Change Directory",
    "cd [TARGET_DIRECTORY_PATH]",
    "Changes the current directory"},
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