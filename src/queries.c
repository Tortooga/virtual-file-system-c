#include "../include/paths.h"
#include "../include/status.h"
#include "../include/folders.h"


StatusCode resolve_folder_path(const char *path, const size_t path_length, Folder *folder)
{
    if (!path || !folder)
    {
        return NULL_POINTER_PASSED;
    }

    //create sub entry search by name
}
/*
Abstraction of files and folders into nodes in the hopes of achieving polymorphic bahaviour for search functionality will add unnecassary complexity.
the following implementation of sub entry search, while redundant, is simple and readable.
*/
//search for sub folder in current_folder by name
StatusCode search_sub_folder(Folder *current_folder, const char *target_folder_name, size_t target_folder_name_length, Folder **out_result_folder_addrs)
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


StatusCode search_sub_file(Folder *current_folder, const char *target_file_name, size_t target_file_name_length, File **out_result_file_addrs)
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