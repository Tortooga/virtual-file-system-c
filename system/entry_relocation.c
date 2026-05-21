#include "entry_relocation.h"
#include "files.h"
#include "folders.h"

#include <string.h>

StatusCode validate_no_cyclic_move(Folder *new_parent, Folder *folder, size_t step_count);

StatusCode rename_file(
    File *file,
    char *name,
    const size_t name_length
)
{
    if (!file || !name)
    {
        return NULL_POINTER_PASSED;
    }

    if (!file->parent_folder)
    {
        return INVALID_ARGUMENT;
    }

    /*  
        Verifies that name is available and is of valid length  
        Will return repetition error if the new name is equal to the current name
    */
    StatusCode status = validate_sub_entry_name(
        file->parent_folder,
        name,
        name_length
    );

    if (status != SUCCESS)
    {
        return status;
    }

    /*
        Enforces null termination.
        so if the old name is longer than the new one, 
        the rest of the old name will be cut off by the null terminator
    */
    status = set_entry_name(
        name,
        name_length,
        file->name
    );

    return status;
}

StatusCode rename_folder(
    Folder *folder,
    char *name,
    const size_t name_length
)
{
    if (!folder || !name)
    {
        return NULL_POINTER_PASSED;
    }

    if (folder->is_root)
    {
        return INVALID_ROOT_OPERATION;
    }

    if (!folder->parent_folder)
    {
        return INVALID_ARGUMENT;
    }

    StatusCode status = validate_sub_entry_name(
        folder->parent_folder,
        name,
        name_length
    );

    if (status != SUCCESS)
    {
        return status;
    }

    status = set_entry_name(
        name,
        name_length,
        folder->name
    );

    return status;
}

//Is not responsible for verifying that new_parent is a part of the same vfs as file.
StatusCode move_file(File *file, Folder *new_parent)
{
    if (!file || !new_parent)
    {
        return NULL_POINTER_PASSED;
    }

    //verifies that the file->name is unique under new_parent
    //file->name is guarnteed to be null terminated
    StatusCode status = validate_sub_entry_name(
        new_parent,
        file->name,
        strlen(file->name)
    );

    if (status != SUCCESS)
    {
        return status;
    }

    size_t new_file_index;

    status = get_available_sub_file_position(
        new_parent,
        &new_file_index
    );

    if (status != SUCCESS)
    {
        return status;
    }

    status = unlink_sub_file(file);

    if (status != SUCCESS)
    {
        return status;
    }

    new_parent->sub_files[new_file_index] = file;
    file->parent_folder = new_parent;

    return SUCCESS;
}

StatusCode move_folder(Folder *folder, Folder *new_parent)
{
    if (!folder || !new_parent)
    {
        return NULL_POINTER_PASSED;
    }

    //verifies that the folder->name is unique under new_parent
    //folder->name is guarnteed to be null terminated
    StatusCode status = validate_sub_entry_name(
        new_parent,
        folder->name,
        strlen(folder->name)
    );

    if (status != SUCCESS)
    {
        return status;
    }

    //making sure new_parent is not a decsendant of folder
    status = validate_no_cyclic_move(
        new_parent,
        folder,
        0
    );

    if (status != SUCCESS)
    {
        return status;
    }

    size_t new_folder_index;

    status = get_available_sub_folder_position(
        new_parent,
        &new_folder_index
    );

    if (status != SUCCESS)
    {
        return status;
    }

    //force is set to true
    //we unlink regardless of whether there are children. We will relink it to new_parent later
    status = unlink_sub_folder(folder, true);

    if (status != SUCCESS)
    {
        return status;
    }

    new_parent->sub_folders[new_folder_index] = folder;
    folder->parent_folder = new_parent;

    return SUCCESS;
}

//Makes sure new_parent is not a descendant of folder
StatusCode validate_no_cyclic_move(Folder *new_parent, Folder *folder, size_t step_count)
{
    if (step_count >= MAX_FOLDER_DEPTH)
    {
        return RECURSIVE_OPERATION_LIMIT_EXCEEDED;
    }

    if (!new_parent || !folder)
    {
        return NULL_POINTER_PASSED;
    }

    if (new_parent == folder)
    {
        return ATTEMPTED_TO_ENFORCE_CYCLIC_RELATION;
    }

    StatusCode status;
    for (size_t i = 0; i < MAX_SUB_FOLDERS_AMOUNT; i++)
    {
        if (folder->sub_folders[i] == NULL)
        {
            continue;
        }

        status = validate_no_cyclic_move(
            new_parent, 
            folder->sub_folders[i], 
            step_count + 1
        );

        if (status != SUCCESS)
        {
            return status;
        }
    }

    return SUCCESS;
}