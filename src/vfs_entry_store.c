#include "../include/files.h"
#include "../include/folders.h"
#include "../include/vfs_entry_store.h"

#include <stdbool.h>

StatusCode get_available_allocation_map_position(
    size_t *out_position, 
    const bool *allocation_map,
    const size_t allocation_map_length);
StatusCode get_available_file_position(size_t *out_file_store_position, const VFSEntryStore *entry_store);


StatusCode vfs_entry_store_init(VFSEntryStore *out_vfs_entry_store)
{
    if (!out_vfs_entry_store)
    {
        return NULL_POINTER_PASSED;
    }

    for (size_t i = 0; i < VFS_MAX_FILES_AMOUNT; i++)
    {
        out_vfs_entry_store->files_allocation_map[i] = false;
    }

    for (size_t i = 0; i < VFS_MAX_FOLDERS_AMOUNT; i++)
    {
        out_vfs_entry_store->folders_allocation_map[i] = false;
    }

    StatusCode status = root_folder_init(&out_vfs_entry_store->root);

    
    return status;
}


//parent_folder must be in entry_store->folders
StatusCode vfs_sub_file_init(
    VFSEntryStore *entry_store,
    char *file_name,
    size_t file_name_length,
    Folder *parent_folder
)
{
    if (!entry_store || !file_name || !parent_folder)
    {
        return NULL_POINTER_PASSED;
    }

    if (parent_folder < entry_store->folders || parent_folder >= entry_store->folders + VFS_MAX_FOLDERS_AMOUNT)
    {
        return UNKNOWN_ENTRY;
    }

    //making sure the supplied parent folder is a valid comitted folder in entry_store->folders
    if (!entry_store->folders_allocation_map[parent_folder - &entry_store->folders[0]])
    {
        return UNINITIALISED_ENTRY;
    }
    
    size_t file_pos;
    StatusCode status = get_available_file_position(&file_pos, entry_store);

    if (status != SUCCESS)
    {
        return status;
    }

    //name validation is done by file_init
    status = file_init(
        &entry_store->files[file_pos],
        file_name,
        file_name_length
    );

    if (status != SUCCESS)
    {
        return status;
    }

    //identifier availability is validated by sub_file_init
    status = sub_file_init(
        &entry_store->files[file_pos],
        parent_folder
    );

    if (status != SUCCESS)
    {
        return status;
    }

    //marked as occupied only after complete success
    entry_store->files_allocation_map[file_pos] = true;
    return SUCCESS;
}

//failure leaves out_file_store_position uninitialised
StatusCode get_available_file_position(size_t *out_file_store_position, const VFSEntryStore *entry_store)
{
    if (!out_file_store_position || !entry_store)
    {
        return NULL_POINTER_PASSED;
    }

    StatusCode status = get_available_allocation_map_position(
        out_file_store_position,
        entry_store->files_allocation_map,
        VFS_MAX_FILES_AMOUNT
    );

    return status;
}

//failure leaves out_folder_store_position uninitialised
StatusCode get_available_folder_position(size_t *out_folder_store_position, const VFSEntryStore *entry_store)
{
    if (!out_folder_store_position || !entry_store)
    {
        return NULL_POINTER_PASSED;
    }

    StatusCode status = get_available_allocation_map_position(
        out_folder_store_position,
        entry_store->folders_allocation_map,
        VFS_MAX_FOLDERS_AMOUNT
    );

    return status;
}

StatusCode get_available_allocation_map_position(
    size_t *out_position, 
    const bool *allocation_map,
    const size_t allocation_map_length)
{
    if (!out_position || !allocation_map)
    {
        return NULL_POINTER_PASSED;
    }

    for (size_t i = 0; i < allocation_map_length; i++)
    {
        if (!allocation_map[i])
        {
            *out_position = i;
            return SUCCESS;
        }
    }

    return NO_SPACE;
}