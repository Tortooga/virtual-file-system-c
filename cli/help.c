#include "help.h"
#include "status.h"
#include "commands.h"

#include <stddef.h>
#include <stdio.h>


StatusCode print_help_hint_format(const HelpHint *help_hint);
StatusCode get_command_function_help_hint(CommandFunction command_function, const HelpHint **help_hint);


StatusCode print_str_command_function_help_hint(char *command_function_string)
{
    if (!command_function_string)
    {
        return NULL_POINTER_PASSED;
    }

    CommandFunction command_function;

    StatusCode status = resolve_cmd_function(
        command_function_string,
        &command_function
    );

    if (status != SUCCESS)
    {
        return status;
    }

    const HelpHint *help_hint;

    status = get_command_function_help_hint(command_function, &help_hint);

    if (status != SUCCESS)
    {
        return status;
    }

    return print_help_hint_format(help_hint);
}


StatusCode get_command_function_help_hint(CommandFunction command_function, const HelpHint **out_help_hint)
{
    if (!out_help_hint)
    {
        return NULL_POINTER_PASSED;
    }
    
    for (size_t i = 0; i < HELP_HINTS_TABLE_LENGTH; i++)
    {
        if (HELP_HINTS_TABLE[i].func == command_function)
        {
            *out_help_hint = &HELP_HINTS_TABLE[i];
            return SUCCESS;
        }
    }

    return COMMAND_FUNCTION_NOT_FOUND;
}


StatusCode print_help_hint_format(const HelpHint *help_hint)
{
    if (!help_hint)
    {
        return NULL_POINTER_PASSED;
    }

    printf(
        "Name\n"
        "   %s\n\n"
        "Synopsis\n"
        "   %s\n\n"
        "Description\n"
        "   %s\n",
        help_hint->name, help_hint->synopsis, help_hint->description
    );

    return SUCCESS;
}