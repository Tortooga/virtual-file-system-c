#include "files.h"
#include "folders.h"
#include "vfs_entry_store.h"

#include "file_logic.h" //only used for deleting a files content

#include <stdbool.h>
#include <stdio.h>

StatusCode get_available_allocation_map_position(
    size_t *out_position, 
    const bool *allocation_map,
    const size_t allocation_map_length);
StatusCode get_available_file_position(size_t *out_file_store_position, const VFSEntryStore *entry_store);
StatusCode get_available_folder_position(size_t *out_folder_store_position, const VFSEntryStore *entry_store);
StatusCode validate_vfs_folder(Folder *folder, VFSEntryStore *entry_store);
StatusCode delete_flat_folder(Folder *folder, VFSEntryStore *entry_store);
StatusCode delete_children_recursive(Folder *cur_folder, VFSEntryStore *entry_store, StorageMan *storage_man, size_t cur_step);

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
    File **out_file,
    VFSEntryStore *entry_store,
    char *file_name,
    size_t file_name_length,
    Folder *parent_folder
)
{
    if (!entry_store || !file_name || !parent_folder || !out_file)
    {
        return NULL_POINTER_PASSED;
    }
    //In case of partial failure 
    *out_file = NULL;

    //verifies that the folder belongs to and is committed to VFS 
    StatusCode status = validate_vfs_folder(parent_folder, entry_store);

    if (status != SUCCESS)
    {
        return status;
    }

    size_t file_pos;
    status = get_available_file_position(&file_pos, entry_store);

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

    //file address placed onto output parameter
    *out_file = &entry_store->files[file_pos];
    //marked as occupied(visible) only after complete success
    entry_store->files_allocation_map[file_pos] = true;
    return SUCCESS;
}


StatusCode vfs_sub_folder_init(
    Folder **out_folder,
    VFSEntryStore *entry_store,
    char *folder_name,
    size_t folder_name_length,
    Folder *parent_folder
)
{
    if (!entry_store || !folder_name || !parent_folder || !out_folder)
    {
        return NULL_POINTER_PASSED;
    }
    //In case of partial failure 
    *out_folder = NULL;

    //verifies that the folder belongs to and is committed to VFS 
    StatusCode status = validate_vfs_folder(parent_folder, entry_store);

    if (status != SUCCESS)
    {
        return status;
    }

    size_t folder_pos;
    status = get_available_folder_position(
        &folder_pos,
        entry_store
    );

    if (status != SUCCESS)
    {
        return status;
    }

    status = sub_folder_init(
        &entry_store->folders[folder_pos], 
        folder_name, 
        folder_name_length, 
        parent_folder
    );

    if (status != SUCCESS)
    {
        return status;
    }

    *out_folder = &entry_store->folders[folder_pos];
    entry_store->folders_allocation_map[folder_pos] = true;
    return SUCCESS;
}

//Unlinks folder from parent and labels its position as available in the allocation map
StatusCode delete_vfs_folder(Folder *folder, VFSEntryStore *entry_store, StorageMan *storage_man, bool force)
{
    if (!folder || !entry_store || !storage_man)
    {
        return NULL_POINTER_PASSED;
    }

    StatusCode status;

    //Checking if the folder is an established entry store entry.
    status = validate_vfs_folder(folder, entry_store);

    if (status != SUCCESS)
    {
        return status;
    }

    bool folder_has_sub_entries;

    status = has_sub_entries(folder ,&folder_has_sub_entries);

    if (status != SUCCESS)
    {
        return status;
    }

    if (!folder_has_sub_entries)
    {
        return delete_flat_folder(
            folder,
            entry_store
        );
    }

    if (!force)
    {
        return ATTEMPTED_TO_DELETE_FOLDER_WITH_SUB_ENTRIES;
    }

    status = delete_children_recursive(
        folder,
        entry_store,
        storage_man, 0
    );
    
    //Fetal
    if (status != SUCCESS)
    {
        return status;
    }

    return SUCCESS;
}


//Private helper
//folder must be a memeber of entry_store->folders_allocation_map 
StatusCode delete_flat_folder(Folder *folder, VFSEntryStore *entry_store)
{
    if (!folder)
    {
        return NULL_POINTER_PASSED;
    }

    //force setting is set to false
    //this means the function will not unlink and will return an error if folder has children
    StatusCode status = unlink_sub_folder(folder, false);
        
    if (status != SUCCESS)
    {
        return status;
    }

    entry_store->folders_allocation_map[folder - &entry_store->folders[0]] = false;

    return SUCCESS;
}

StatusCode delete_children_recursive(Folder *cur_folder, VFSEntryStore *entry_store, StorageMan *storage_man, size_t cur_step)
{
    if (!cur_folder || !entry_store || !storage_man)
    {
        return NULL_POINTER_PASSED;
    }

    if (cur_step > MAX_FOLDER_DEPTH)
    {
        return RECURSIVE_OPERATION_LIMIT_EXCEEDED;
    }

    cur_step++;

    StatusCode status;

    status = validate_vfs_folder(cur_folder, entry_store);

    if (status != SUCCESS)
    {
        return status;
    }

    /* 
        Any failure is fatal.
    */
    
    for (size_t i = 0; i < MAX_SUB_FILES_AMOUNT; i++)
    {
        if (cur_folder->sub_files[i] == NULL)
        {
            continue;
        }
        
        status = delete_vfs_file(entry_store, cur_folder->sub_files[i], storage_man);

        if (status != SUCCESS)
        {
            return status;
        }
    }

    for (size_t i = 0; i < MAX_SUB_FOLDERS_AMOUNT; i++)
    {
        if (cur_folder->sub_folders[i] == NULL)
        {
            continue;
        }

        status = delete_children_recursive(cur_folder->sub_folders[i], entry_store, storage_man, cur_step);
        
        if (status != SUCCESS)
        {
            return status;
        }
    }
    
    //Children have already been deleted. Folder has become flat
    //validation guarantees folder is within &entry_store->folders
    status = delete_flat_folder(
        cur_folder,
        entry_store
    );

    return status;
}

StatusCode delete_vfs_file(VFSEntryStore *entry_store, File *file, StorageMan *storage_man)
{
    if (!entry_store || !file)
    {
        return NULL_POINTER_PASSED;
    }

    StatusCode status;

    //Checking if parent is an established entry store entry
    status = validate_vfs_folder(file->parent_folder, entry_store);

    if (status != SUCCESS)
    {
        return status;
    }

    if (&entry_store->files[0] > file || file >= &entry_store->files[0] + VFS_MAX_FILES_AMOUNT)
    {
        return UNKNOWN_ENTRY;
    }
    
    status = unlink_sub_file(file);

    if (status != SUCCESS)
    {
        return status;
    }

    status = file_delete_data(file, storage_man);

    if (status != SUCCESS)
    {
        return status;
    }

    entry_store->files_allocation_map[file - &entry_store->files[0]] = false;

    return SUCCESS;
}


//verifies that the folder belongs to and is committed to VFS 
StatusCode validate_vfs_folder(Folder *folder, VFSEntryStore *entry_store)
{
    //Caller guarantees not NULL pointers

    //If the folder address is the address of the root folder succeed
    //root folder validity is guaranteed by VFS initialiser
    if (folder == &entry_store->root)
    {
        return SUCCESS;
    }

    if (folder < entry_store->folders || folder >= entry_store->folders + VFS_MAX_FOLDERS_AMOUNT)
    {
        return UNKNOWN_ENTRY;
    }

    //making sure the supplied parent folder is a valid comitted folder in entry_store->folders
    if (!entry_store->folders_allocation_map[folder - &entry_store->folders[0]])
    {
        return UNINITIALISED_ENTRY;
    }

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


/* DEBUG FUNCTIONS */


StatusCode print_entry_store(VFSEntryStore *entry_store)
{
    if (!entry_store)
    {
        return NULL_POINTER_PASSED;
    }

    printf("Printing VFS Entry Store...\n");
    
    printf("File Allocation Map: ");
    for (size_t i = 0; i < VFS_MAX_FILES_AMOUNT; i++)
    {
        printf(" %d", entry_store->files_allocation_map[i]);
    }
    printf("\n");

    printf("Folder Allocation Map: ");
    for (size_t i = 0; i < VFS_MAX_FOLDERS_AMOUNT; i++)
    {
        printf(" %d", entry_store->folders_allocation_map[i]);
    }
    printf("\n");

    printf("Tree:\n");
    StatusCode status = print_folder(&entry_store->root);
    return status;
}