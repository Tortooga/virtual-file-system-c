#include "../include/queries.h"
#include "../include/path_utils.h"
#include "../include/status.h"
#include "../include/folders.h"

#include <string.h>

StatusCode get_vfs_node(const char *name, Folder *cur_folder, VFSNode *out_vfs_node);

//path always starts at (but does not include) the root folder
//returns a node containing either a file or a folder(see VFSNode definition)
//Does not assume or enforce path name NULL termination 
//Failed search does not initialise out_node
StatusCode resolve_path(
    const char *path, 
    const size_t path_length,
    Folder *root_folder, 
    VFSNode *out_vfs_node)
{
    if (!path || !out_vfs_node || !root_folder)
    {
        return NULL_POINTER_PASSED;
    }
    
    if (path_length == 0)
    {
        return PATH_IS_EMPTY;
    }
    
    size_t nodes_amount;
    char node_names[MAX_PATH_NODES_AMOUNT][MAX_NAME_LENGTH];
    
    //node_names guaranteed to be null terminated
    StatusCode status = parse_path(
        path,
        path_length,
        &nodes_amount,
        MAX_NAME_LENGTH,
        MAX_PATH_NODES_AMOUNT,
        node_names
    );

    if (status != SUCCESS)
    {
        return status;
    }

    //Success of parse_path guarantees at least one node
    //We will check anyway
    if (nodes_amount == 0)
    {
        return PATH_IS_EMPTY;
    }
    
    //path always starts at the root
    Folder *cur_folder = root_folder;

    //We subtract one since the last node is treated differently(it may or may not be a file)
    for (size_t i = 0; i < nodes_amount - 1; i++)
    {
        //path parser guarantees node names are null terminated
        status = search_sub_folder(
            cur_folder,
            node_names[i],
            strlen(node_names[i]),
            &cur_folder
        );

        if (status != SUCCESS)
        {
            return status;
        }
    }

    status = get_vfs_node(
        node_names[nodes_amount - 1], //last node name
        cur_folder,
        out_vfs_node
    );

    return status;
}

//name is null terminated(guaranteed by path_parser)
//the sub_entry initialisers guarantee that there are no name collisions even accross files and folders
StatusCode get_vfs_node(const char *name, Folder *cur_folder, VFSNode *out_vfs_node)
{
    if (!name || !cur_folder || !out_vfs_node)
    {
        return NULL_POINTER_PASSED;
    }

    const size_t name_length = strlen(name);
    File *file_addrs;
    StatusCode status;

    status = search_sub_file
    (
        cur_folder,
        name,
        name_length,
        &file_addrs
    );

    //if the search succeeds then the node is a file
    if (status == SUCCESS)
    {
        out_vfs_node->type = FILE_NODE;
        out_vfs_node->node.file = file_addrs;
        
        return SUCCESS;
    }

    //if the search fails for any reason other than not finding a file with the required name we exit
    if (status != SEARCH_TARGET_NOT_FOUND)
    {
        return status;
    }

    Folder *folder_addrs;
    status = search_sub_folder
    (
        cur_folder,
        name,
        name_length,
        &folder_addrs
    );

    if (status != SUCCESS)
    {
        return status;
    }

    out_vfs_node->type = FOLDER_NODE;
    out_vfs_node->node.folder = folder_addrs;
    return SUCCESS;
} 

/*
Abstraction of files and folders into nodes in the hopes of achieving polymorphic bahaviour for search functionality will add unnecassary complexity.
the following implementation of sub entry search, while redundant, is simple and readable.
*/
//search for sub folder in current_folder by name
StatusCode search_sub_folder(const Folder *current_folder, const char *target_folder_name, const size_t target_folder_name_length, Folder **out_result_folder_addrs)
{
    if (!current_folder || !target_folder_name || !out_result_folder_addrs)
    {
        return NULL_POINTER_PASSED;
    }

    if (target_folder_name_length == 0 || target_folder_name_length > MAX_NAME_LENGTH)
    {
        return INVALID_ARGUMENT;
    }

    bool cmp_result;
    StatusCode status;
    for (size_t i = 0; i < MAX_SUB_FOLDERS_AMOUNT; i++)
    {
        //skip vacant sub_folder positions
        if (current_folder->sub_folders[i] == NULL)
        {
            continue;
        }

        //compare names
        status = name_eq(current_folder->sub_folders[i]->name, target_folder_name, target_folder_name_length, &cmp_result);
        
        if (status != SUCCESS)
        {
            return status;
        }

        if (cmp_result == true)
        {
            *out_result_folder_addrs = current_folder->sub_folders[i];
            return SUCCESS;
        }
    }

    *out_result_folder_addrs = NULL;
    return SEARCH_TARGET_NOT_FOUND;
}


StatusCode search_sub_file(const Folder *current_folder, const char *target_file_name, const size_t target_file_name_length, File **out_result_file_addrs)
{
    if (!current_folder || !target_file_name || !out_result_file_addrs)
    {
        return NULL_POINTER_PASSED;
    }

    if (target_file_name_length == 0 || target_file_name_length > MAX_NAME_LENGTH)
    {
        return INVALID_ARGUMENT;
    }

    bool cmp_result;
    StatusCode status;
    for (size_t i = 0; i < MAX_SUB_FILES_AMOUNT; i++)
    {
        //skip vacant sub_files positions
        if (current_folder->sub_files[i] == NULL)
        {
            continue;
        }

        //compare names
        status = name_eq(current_folder->sub_files[i]->name, target_file_name, target_file_name_length, &cmp_result);
        
        if (status != SUCCESS)
        {
            return status;
        }

        if (cmp_result == true)
        {
            *out_result_file_addrs = current_folder->sub_files[i];
            return SUCCESS;
        }
    }

    *out_result_file_addrs = NULL;
    return SEARCH_TARGET_NOT_FOUND;
}