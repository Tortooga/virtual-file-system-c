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

    return SEARCH_TARGET_NOT_FOUND;
}