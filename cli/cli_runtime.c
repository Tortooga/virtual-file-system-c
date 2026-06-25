#include "cli_runtime.h"

#include "status.h"
#include "storage.h"
#include "vfs_entry_store.h"

#include "workspace.h"

#include "input_handler.h"
#include "commands.h"
#include "command_dispatcher.h"

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

void print_prompt_context(Workspace *workspace);
void report_status(StatusCode status);
void print_starting_instructions();


StatusCode cli_run(
    StorageMan *storage_man, 
    VFSEntryStore *entry_store, 
    Workspace *workspace
)
{
    if (!storage_man || !entry_store || !workspace)
    {
        return NULL_POINTER_PASSED;
    }

    char *output_buffer = malloc(OUTPUT_BUFFER_SIZE);

    if (!output_buffer)
    {
        return ALLOCATION_FAILED;
    }

    InputHandler input_handler;

    bool exit_flag = false;

    StatusCode status;

    Command cur_command;

    print_starting_instructions();

    while (!exit_flag)
    {
        print_prompt_context(workspace);

        fflush(stdout);

        status = new_line_terminated_read(
            input_handler.command_buffer, 
            COMMAND_BUFFER_SIZE
        );

        if (status != SUCCESS)
        {
            report_status(status);
            return status;
        }

        status = cmd_parse(input_handler.command_buffer, &cur_command);

        if (status != SUCCESS)
        {
            report_status(status);
            continue;
        }

        status = command_dispatch(
            workspace,
            &cur_command,
            &input_handler,
            output_buffer,
            OUTPUT_BUFFER_SIZE,
            &exit_flag
        );

        if (exit_flag)
        {
            break;
        }

        report_status(status);
    }

    return SUCCESS;
}

void print_starting_instructions()
{
    printf("When in multiline read mode(such as when you call append), use CTRL + D on empty line to submit.\n\n");
}

void print_prompt_context(Workspace *workspace)
{
    //Workspace guaranteed not to be null

    printf("VFS:%s# ", workspace->cur_path);
}

void report_status(StatusCode status)
{
    if (status == SUCCESS)
    {
        //SUCCESS does not get reported
        return;
    }

    printf("Status [%d]\n", status);
}