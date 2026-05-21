#include "queries.h"
#include "folders.h"

#include <stdio.h>
#include <string.h>

StatusCode test_search_sub_folder()
{
    const size_t folders_amount = 10;
    Folder folders[folders_amount];
    Folder root;
    
    StatusCode status = root_folder_init(&root);

    if (status != SUCCESS)
    {
        return status;
    }

    char name_buffer[8];
    const size_t name_buffer_length = sizeof(name_buffer) / sizeof(name_buffer[0]);
    for (size_t i = 0; i < folders_amount; i++)
    {
        snprintf(name_buffer, name_buffer_length, "test%zu", i);
        status = sub_folder_init(&folders[i], name_buffer, name_buffer_length, &root);

        if (status != SUCCESS)
        {
            return status;
        }
    }

    Folder *cur_out_folder;
    for (size_t i = 0; i < folders_amount; i++)
    {
        snprintf(name_buffer, name_buffer_length, "test%zu", i);
        status = search_sub_folder(
            &root,
            name_buffer,
            name_buffer_length,
            &cur_out_folder
        );

        if (status != SUCCESS)
        {
            return status;
        }

        if (cur_out_folder != &folders[i])
        {
            return TEST_ASSERTION_FAILED;
        }
    }

    return SUCCESS;
}