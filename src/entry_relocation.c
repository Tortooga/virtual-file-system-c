#include "../include/entry_relocation.h"
#include "../include/files.h"
#include "../include/folders.h"

#include <string.h>

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

//Moves file to new parent 
//Ensures that file.name is unique under new parent
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