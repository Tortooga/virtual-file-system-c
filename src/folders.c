#include "../include/folders.h"
#include "../include/status.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static const char root_name[] = "root";
size_t root_name_length = sizeof(root_name) - 1;

StatusCode get_available_sub_folder_position(Folder *parent_folder, size_t *out_index);
StatusCode get_available_sub_file_position(Folder *parent_folder, size_t *out_index);
StatusCode folder_free_sub_entries(Folder *folder);
StatusCode name_eq(const char *initialised_name, const char *uninitialised_name, size_t uninitialised_name_length, bool *out_result);
StatusCode sub_entry_name_is_unique(Folder *parent_folder, const char *name, size_t name_length, bool *out_result);
StatusCode has_sub_entries(Folder *folder, bool *out_result);
StatusCode find_sub_folder_index(Folder *sub_folder, size_t *out_sub_folder_index);
StatusCode find_sub_file_index(Folder *parent_folder, File *file, size_t *out_sub_file_index);


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

//initialises a file into a sub_file
//file guaranteed to have been initialised through file_init
//for that purpose this function skips name and chunk_extent initialisation
//this function, however, enforces name uniqueness across all sub-entries 
StatusCode sub_file_init(
    File *file,
    Folder *parent_folder
)
{
    if (!file || !parent_folder)
    {
        return NULL_POINTER_PASSED;
    }

    //checking if the files name is available
    //file->name guaranteed to be null terminated
    StatusCode status = validate_sub_entry_name(
        parent_folder,
        file->name,
        strlen(file->name)
    );

    if (status != SUCCESS)
    {
        return status;
    }

    size_t sub_file_index;
    status = get_available_sub_file_position(parent_folder, &sub_file_index);

    if (status != SUCCESS)
    {
        return status;
    }

    parent_folder->sub_files[sub_file_index] = file;
    file->parent_folder = parent_folder;
    return SUCCESS;
}

//Creates sub_folder
//Initialises sub_entries to NULL
//name is not assumed to be a C-string but will work as a C-string
//root initialisation is handled seperately
StatusCode sub_folder_init(
    Folder *folder,
    const char *name,
    const size_t name_length,
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

//TODO remove parent_folder once it is added as an attribute of files
StatusCode unlink_sub_file(Folder *parent_folder, File *file)
{
    if (!parent_folder || !file)
    {
        return NULL_POINTER_PASSED;
    }

    size_t sub_file_index;
    StatusCode status = find_sub_file_index(parent_folder, file, &sub_file_index);

    if (status != SUCCESS)
    {
        return status;
    }

    parent_folder->sub_files[sub_file_index] = NULL;
    file->parent_folder = NULL;
    return SUCCESS;
}


//unlinks sub_folder from parent
//will not unlink sub_folder if it has  sub entries unless force is set to true
StatusCode unlink_sub_folder(Folder *sub_folder, bool force)
{
    if (!sub_folder)
    {
        return NULL_POINTER_PASSED;
    }

    if (sub_folder->is_root)
    {
        return INVALID_ROOT_OPERATION;
    }

    bool sub_folder_has_sub_entries;

    StatusCode status = has_sub_entries(sub_folder, &sub_folder_has_sub_entries);

    if (status != SUCCESS)
    {
        return status;
    }

    if (sub_folder_has_sub_entries && !force)
    {
        return ATTEMPTED_TO_DELETE_FOLDER_WITH_SUB_ENTRIES;
    }

    //the sub_folders index within parent.sub_folders
    size_t sub_folder_index;  
    status = find_sub_folder_index(sub_folder, &sub_folder_index);
    
    if (status != SUCCESS)
    {
        return status;
    }

    sub_folder->parent_folder->sub_folders[sub_folder_index] = NULL;
    sub_folder->parent_folder = NULL;
    
    return SUCCESS;
}

//finds sub_folders index within sub_folder->parent_folder.sub_folders[]
StatusCode find_sub_folder_index(Folder *sub_folder, size_t *out_sub_folder_index)
{
    if (!out_sub_folder_index || !sub_folder)
    {
        return NULL_POINTER_PASSED;
    }

    if (sub_folder->is_root)
    {
        return INVALID_ROOT_OPERATION;
    }

    for (size_t i = 0; i < MAX_SUB_FOLDERS_AMOUNT; i++)
    {
        if (sub_folder->parent_folder->sub_folders[i] == sub_folder)
        {
            *out_sub_folder_index = i;
            return SUCCESS;
        }
    }

    return SEARCH_TARGET_NOT_FOUND;
}

StatusCode find_sub_file_index(Folder *parent_folder, File *file, size_t *out_sub_file_index)
{
    if (!parent_folder || !file || !out_sub_file_index)
    {
        return NULL_POINTER_PASSED;
    }

    for (size_t i = 0; i < MAX_SUB_FILES_AMOUNT; i++)
    {
        if (parent_folder->sub_files[i] == file)
        {
            *out_sub_file_index = i;
            return SUCCESS;
        }
    }

    return SEARCH_TARGET_NOT_FOUND;
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

//failure initialises out_index to SIZE_MAX
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

//same pattern, different domain

//failure initialises out_index to SIZE_MAX
StatusCode get_available_sub_file_position(Folder *parent_folder, size_t *out_index)
{
    //Vacancy in Folder.sub_folder and Folder.sub_file is denoted by NULL pointers
    if (!parent_folder || !out_index)
    {
        return NULL_POINTER_PASSED;
    }

    for (size_t i = 0; i < MAX_SUB_FILES_AMOUNT; i++)
    {
        if (parent_folder->sub_files[i] == NULL)
        {
            *out_index = i;
            return SUCCESS;
        }
    }

    *out_index = SIZE_MAX;
    return NO_SPACE;
}