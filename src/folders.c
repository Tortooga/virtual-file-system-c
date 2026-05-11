#include "../include/folders.h"
#include "../include/status.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static const char root_name[] = "root";
size_t root_name_length = sizeof(root_name) - 1;

StatusCode get_available_sub_folder_position(Folder *parent_folder, size_t *out_index);
StatusCode folder_free_sub_entries(Folder *folder);
StatusCode name_eq(const char *initialised_name, const char *uninitialised_name, size_t uninitialised_name_length, bool *out_result);
StatusCode sub_entry_name_is_unique(Folder *parent_folder, const char *name, size_t name_length, bool *out_result);
StatusCode validate_sub_entry_name(
    Folder *parent_folder,
    const char *entry_name,
    const size_t entry_name_length
);
StatusCode set_entry_name(
    const char *name,
    const size_t name_length,
    char *out_entry_name
);

//Initialises folder into root directory
StatusCode root_folder_init(Folder *folder)
{
    if (!folder)
    {
        return NULL_POINTER_PASSED;
    }
    
    //no need for +1 to account for null terminator as folder.name is of size MAX_NAME_LENGTH + 1 to account for it
    if (root_name_length > MAX_NAME_LENGTH)
    {
        return DATA_OVER_FLOW;
    }
    
    StatusCode status = folder_free_sub_entries(folder);

    if (status != SUCCESS)
    {
        return status;
    }

    //static const root_name guaranteed to be null terminated
    strcpy(folder->name, root_name);
    
    folder->is_root = true;
    
    //root has no parent
    folder->parent_folder = NULL;

    return SUCCESS;
}


//Creates sub_folder
//Initialises sub_entries to NULL
//name is not assumed to be a C-string but will work as a C-string
//root initialisation is handled seperately
StatusCode sub_folder_init(
    Folder *folder,
    const char *name,
    size_t name_length,
    Folder *parent_folder
)
{
    if (!name || !folder || !parent_folder)
    {
        return NULL_POINTER_PASSED;
    }

    StatusCode status = validate_sub_entry_name(
        parent_folder,
        name,
        name_length
    );

    if (status != SUCCESS)
    {
        return status;
    }

    size_t sub_folder_index;
    status = get_available_sub_folder_position(parent_folder, &sub_folder_index);
    
    if (status != SUCCESS)
    {
        return status;
    }
    
    status = set_entry_name(
        name,
        name_length,
        folder->name
    );

    if (status != SUCCESS)
    {
        return status;
    }

    //root initialisation is handled differently
    folder->is_root = false;

    status = folder_free_sub_entries(folder);
    if (status != SUCCESS)
    {
        return status;
    }

    folder->parent_folder = parent_folder;
    parent_folder->sub_folders[sub_folder_index] = folder;
    return SUCCESS;
}

//Sets an entries name
StatusCode set_entry_name(
    const char *name,
    const size_t name_length,
    char *out_entry_name
)
{
    //out_entry_name guaranteed to be the entry.name field
    //therefor out_entry_name is of size MAX_NAME_LENGTH + 1
    //name_length is guaranteed to be less than or equal to MAX_NAME_LENGTH
    if (!name || !out_entry_name)
    {
        return NULL_POINTER_PASSED;
    }

    size_t i;
    //End name either when name_length is reached 
    //or when a null terminator is encountered
    for (i = 0; i < name_length; i++)
    {
        if (name[i] == '\0')
        {
            break;
        }
        out_entry_name[i] = name[i];
    }

    //entry.name is of size MAX_NAME_LENGTH + 1
    //so we are always guaranteed space for a null terminator
    out_entry_name[i] = '\0';

    return SUCCESS;
}

//validates a sub entries name in terms of correctness and availability
StatusCode validate_sub_entry_name(
    Folder *parent_folder,
    const char *entry_name,
    const size_t entry_name_length
)
{
    if (!parent_folder || !entry_name)
    {
        return NULL_POINTER_PASSED;
    }

    if (entry_name_length > MAX_NAME_LENGTH)
    {
        return DATA_OVER_FLOW;
    }

    if (entry_name_length == 0)
    {
        return INSUFFICIENT_ARRAY_PASSED;
    }

    //Making sure it is not using the root directoy name
    if (entry_name_length == root_name_length)
    {
        if (strncmp(entry_name, root_name, entry_name_length) == 0)
        {
            return IDENTIFIER_NOT_AVAILABLE;
        }
    }
    

    bool name_is_unique;
    StatusCode status = sub_entry_name_is_unique(parent_folder, entry_name, entry_name_length, &name_is_unique);

    if (status != SUCCESS)
    {
        return status;
    }
    
    if (!name_is_unique)
    {
        return IDENTIFIER_NOT_AVAILABLE;
    }

    return SUCCESS;
}


StatusCode has_sub_entries(Folder *folder, bool *out_result)
{
    if (!folder || !out_result)
    {
        return NULL_POINTER_PASSED;
    }

    //abstraction of these two similar loops is unneccessary complexity

    for (size_t i = 0; i < MAX_SUB_FILES_AMOUNT; i++)
    {
        if (folder->sub_files[i] != NULL)
        {
            *out_result = true;
            return SUCCESS;
        }
    }
    
    for (size_t i = 0; i < MAX_SUB_FOLDERS_AMOUNT; i++)
    {
        if (folder->sub_folders[i] != NULL)
        {
            *out_result = true;
            return SUCCESS;
        } 
    }
    
    *out_result = false;
    return SUCCESS;
}

StatusCode sub_entry_name_is_unique(Folder *parent_folder, const char *name, size_t name_length, bool *out_result)
{
    if (!parent_folder || !name || !out_result)
    {
        return NULL_POINTER_PASSED;
    }
    
    for (size_t i = 0; i < MAX_SUB_FILES_AMOUNT; i++)
    {
        if (parent_folder->sub_files[i] == NULL)
        {
            continue;
        }

        //Not NULL sub files guaranteed to be initialised
        bool names_are_equal;
        StatusCode status = name_eq(parent_folder->sub_files[i]->name, name, name_length, &names_are_equal);
        if (status != SUCCESS)
        {
            return status;
        }

        if (names_are_equal)
        {
            *out_result = false;
            return SUCCESS;
        }
    }
    for (size_t i = 0; i < MAX_SUB_FOLDERS_AMOUNT; i++)
    {
        if (parent_folder->sub_folders[i] == NULL)
        {
            continue;
        }

        //Not NULL sub folders guaranteed to be initialised
        bool names_are_equal;
        StatusCode status = name_eq(parent_folder->sub_folders[i]->name, name, name_length, &names_are_equal);
        if (status != SUCCESS)
        {
            return status;
        }

        if (names_are_equal)
        {
            *out_result = false;
            return SUCCESS;
        }
    }

    *out_result = true;

    return SUCCESS;
}

StatusCode name_eq(const char *initialised_name, const char *uninitialised_name, size_t uninitialised_name_length, bool *out_result)
{
    //initialised_name guaranteed to be null terminated
    //null termination is enforced in the initialisation process

    //uninitialised name not guaranteed to be null terminated
    //uninitialised name terminates at either length or if an early null terminator is encountered

    if (!initialised_name || !uninitialised_name || !out_result)
    {
        return NULL_POINTER_PASSED;
    }

    if (uninitialised_name_length == 0)
    {
        return INSUFFICIENT_ARRAY_PASSED;
    }
    size_t i;
    for (i = 0; i < uninitialised_name_length; i++)
    {
        //if we encounter the end of initialised_name
        if (initialised_name[i] == '\0')
        {
            //if uninitialised_name ends here it means they are equal
            *out_result = (uninitialised_name[i] == '\0');
            
            return SUCCESS;
        }

        if (initialised_name[i] != uninitialised_name[i])
        {
            *out_result = false;
            return SUCCESS;
        }
    }

    //reaching this point implies that the first uninitialised_name_length charecters of initialised name are the equal to uninitialised_name
    //if initialised_name ends here
    *out_result = (initialised_name[i] == '\0');
    
    return SUCCESS;
}

StatusCode folder_free_sub_entries(Folder *folder)
{
    if (!folder)
    {
        return NULL_POINTER_PASSED;
    }

    for (size_t i = 0; i < MAX_SUB_FOLDERS_AMOUNT; i++)
    {
        folder->sub_folders[i] = NULL;
    }

    for (size_t i = 0; i < MAX_SUB_FILES_AMOUNT; i++)
    {
        folder->sub_files[i] = NULL;
    }

    return SUCCESS;
}

StatusCode get_available_sub_folder_position(Folder *parent_folder, size_t *out_index)
{
    //Vacancy in Folder.sub_folder and Folder.sub_file is denoted by NULL pointers
    if (!parent_folder || !out_index)
    {
        return NULL_POINTER_PASSED;
    }

    for (size_t i = 0; i < MAX_SUB_FOLDERS_AMOUNT; i++)
    {
        if (parent_folder->sub_folders[i] == NULL)
        {
            *out_index = i;
            return SUCCESS;
        }
    }

    *out_index = SIZE_MAX;
    return NO_SPACE;
}