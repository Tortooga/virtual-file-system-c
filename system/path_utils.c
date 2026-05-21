#include "files.h"
#include "folders.h"
#include "status.h"

#include <string.h>


StatusCode parse_node(
    const char **cur_path_char,
    const char *path_upper_lim,

    const size_t node_name_buffer_length,
    char *out_node_name_buffer
);

StatusCode get_parent_folders(VFSNode *node, Folder **out_parent_folders, size_t *out_parent_folders_amount);
StatusCode append_node_name_to_path(const char *node_name, char *path_end, const size_t available_bytes_amount);

//path null termination not enforced or assumed at this level
//Individual node names are null terminated
//nodes amount output param indicates length of node list

StatusCode parse_path(
    const char *path,
    const size_t path_length,
    size_t *out_nodes_amount,

    const size_t node_name_buffer_length,
    const size_t node_names_array_length,
    char out_node_names[node_names_array_length][node_name_buffer_length]
)
{
    if (!path || !out_node_names || !out_nodes_amount)
    {
        return NULL_POINTER_PASSED;
    }

    if (node_name_buffer_length == 0 || node_names_array_length == 0)
    {
        return INVALID_ARGUMENT;
    }

    if (path_length == 0)
    {
        return PATH_IS_EMPTY;
    }

    *out_nodes_amount = 0;

    //Output param node iteration array pointer and upperlim
    char (*cur_node)[node_name_buffer_length] = out_node_names;
    char (*node_array_upper_lim)[node_name_buffer_length] = cur_node + node_names_array_length;
    
    //path char pointer and upperlim
    const char *cur_path_char = path;
    const char *path_upper_lim = path + path_length;

    StatusCode status;
    for (;;)
    {
        //We have finished traversing the path
        if (cur_path_char >= path_upper_lim)
        {
            return SUCCESS;
        }

        //Our output buffer can not fit the rest of the data
        if (cur_node >= node_array_upper_lim)
        {
            return PATH_IS_TOO_LONG;
        }

        status = parse_node(
            &cur_path_char,
            path_upper_lim,
            node_name_buffer_length,
            *cur_node //derefernced into pointer to first element
        );

        if (status != SUCCESS)
        {
            return status;
        }

        //We increment once to skip path delimiter
        cur_path_char++;
        
        (*out_nodes_amount)++;
        cur_node++;
    }
}

//node name guaranteed to be null terminated 
//out_path will be null terminated
StatusCode get_node_path(VFSNode *node, char *out_path, size_t path_length)
{
    if (!node || !out_path)
    {
        return NULL_POINTER_PASSED;
    }

    //We add another MAX_PATH_NODES_AMOUNT to include space for all the delimiters
    //another +1 for the null terminator
    if (path_length < MAX_PATH_NODES_AMOUNT * (MAX_NAME_LENGTH + 1) + 1)
    {
        return INSUFFICIENT_ARRAY_PASSED;
    }

    if (node->type == FOLDER_NODE)
    {
        //Root is indicated by the path delimiter without any name
        
        if (node->node.folder->is_root)
        {
            out_path[0] = PATH_DELIMITER;
            out_path[1] = '\0';
            return SUCCESS;
        }
    }

    //full path including node is guaranteed to be of size less than or equal to MAX_PATH_NODES_AMOUNT 
    Folder *parent_folders[MAX_PATH_NODES_AMOUNT - 1];
    size_t parent_folders_amount;
    
    StatusCode status = get_parent_folders(
        node, 
        parent_folders, 
        &parent_folders_amount);

    if (status != SUCCESS)
    {
        return status;
    }

    size_t cur_position_in_path = 0;
    size_t available_bytes_amount = path_length;
    size_t cur_entry_length;

    //cur_position_in_path will never be out of range because:
    //cur_position_in_path <= (MAX_PATH_NODES_AMOUNT) * (MAX_NAME_LENGTH + 1) <= path_length
    for (size_t i = 1; i <= parent_folders_amount; i++)
    {
        
        //append_node_name_to_path handles overflow by taking available_bytes_amount
        //though overflow should be implossible
        //since buffer length is enforced to be greater than or equal to the maximum amount needed

        status = append_node_name_to_path(
            parent_folders[parent_folders_amount - i]->name,
            out_path + cur_position_in_path,
            available_bytes_amount
        );

        if (status != SUCCESS)
        {
            return status;
        }

        //folder names guaranteed to be null terminated
        //+1 to include the path delimiter
        cur_entry_length = strlen(parent_folders[parent_folders_amount - i]->name) + 1;
        
        available_bytes_amount -= cur_entry_length;
        cur_position_in_path += cur_entry_length;
    }
    
    //Now that we have written the parent folders we must write the node

    if (node->type == FILE_NODE)
    {
        status = append_node_name_to_path(
            node->node.file->name,
            out_path + cur_position_in_path,
            available_bytes_amount
        );

        if (status != SUCCESS)
        {
            return status;
        }
    }
    else
    {
        status = append_node_name_to_path(
            node->node.folder->name,
            out_path + cur_position_in_path,
            available_bytes_amount
        );

        if (status != SUCCESS)
        {
            return status;
        }
    }
    
    return SUCCESS;    
}


//out_parent_folders guaranteed to atleast be of size MAX_PATH_NODES_AMOUNT - 1 
//outputs parent folders excluding root
//parents are ordered closest to furthest
StatusCode get_parent_folders(VFSNode *node, Folder **out_parent_folders, size_t *out_parent_folders_amount)
{
    if (!node || !out_parent_folders || !out_parent_folders_amount)
    {
        return NULL_POINTER_PASSED;
    }

    *out_parent_folders_amount = 0;

    //If the node is the root or a child of the root return without writing anything
    //This is because this function excludes the node and the root so there is nothing to write
    if (node->type == FILE_NODE)
    {
        if (node->node.file->parent_folder->is_root)
        {
            return SUCCESS;
        }
        out_parent_folders[0] = node->node.file->parent_folder;
    }
    else
    {
        if (node->node.folder->is_root || node->node.folder->parent_folder->is_root)
        {
            return SUCCESS;
        }
        out_parent_folders[0] = node->node.folder->parent_folder;
    }

    *out_parent_folders_amount = 1;

    //guaranteed to encounter root folder before we hit path length limit
    for (size_t i = 1; i < MAX_PATH_NODES_AMOUNT; i++)
    {   
        if (out_parent_folders[i - 1]->parent_folder->is_root)
        {
            return SUCCESS;
        }

        //folder.parent_folder not null as long as folder.is_root = false
        out_parent_folders[i] = out_parent_folders[i - 1]->parent_folder;
        (*out_parent_folders_amount)++;
    }

    return PATH_IS_TOO_LONG;
}



//node_name is null terminated(enforced in all entry initialisers)
//path_end points to the end of the path(at the null terminator), where we are supposed to start the appended name
//it is important that it points at the null terminator so that it could be overwritten
//available_bytes_amount is the amount of available bytes after and not including null terminator
StatusCode append_node_name_to_path(const char *node_name, char *path_end, const size_t available_bytes_amount)
{
    if (!node_name || !path_end)
    {
        return NULL_POINTER_PASSED;
    }

    if (available_bytes_amount == 0)
    {
        return DATA_OVER_FLOW;
    }

    //node_name guaranteed to be valid and null terminated
    const size_t node_name_length = strlen(node_name);

    //+1 for delimiter
    //No need to add another pos for null terminator since we will overwrite the original null terminator
    if (available_bytes_amount < node_name_length + 1)
    {
        return DATA_OVER_FLOW;
    }

    //Adding the path delimiter
    path_end[0] = PATH_DELIMITER;

    size_t cur_pos;
    for (cur_pos = 0; cur_pos < node_name_length; cur_pos++)
    {
        path_end[cur_pos + 1] = node_name[cur_pos];
    }

    //placing null terminator after the name appended
    path_end[cur_pos + 1] = '\0';

    return SUCCESS;
}

StatusCode parse_node(
    const char **cur_path_char,
    const char *path_upper_lim,

    const size_t node_name_buffer_length,
    char *out_node_name_buffer
)
{
    if (!cur_path_char || !out_node_name_buffer)
    {
        return NULL_POINTER_PASSED;
    }

    //Upper limit for out_node_name_buffer after which we exceed the memory allocated for this node name
    //We subtract one to make space for the null terminator to be written at the end of each node name
    const char *node_name_buffer_upper_lim = out_node_name_buffer + node_name_buffer_length - 1;

    for (;;)
    {
        //If we reach the end of the path or encounter delimiter the node ends
        if (*cur_path_char >= path_upper_lim || **cur_path_char == PATH_DELIMITER)
        {
            //Writing null terminator at the end of the node name
            *out_node_name_buffer = '\0';

            return SUCCESS;
        }

        //Node name is longer than the buffer allocated 
        if (out_node_name_buffer >= node_name_buffer_upper_lim)
        {
            return IDENTIFIER_IS_TOO_LONG;
        }
        
        *out_node_name_buffer = **cur_path_char;
        
        (*cur_path_char)++;
        out_node_name_buffer++;
    }
}

//path is null terminated 
//path_string_length is the length of the path until the first occurence of the null terminator
//empty path(which corsponds to the root folder) leads to no op success
StatusCode truncate_path(char *path, size_t path_string_length)
{
    if (!path)
    {
        return NULL_POINTER_PASSED;
    }

    if (path_string_length == 0)
    {
        return SUCCESS;
    }

    char *cur_char = path + path_string_length - 1;

    for (;; cur_char--)
    {
        if (*cur_char == PATH_DELIMITER)
        {
            *cur_char = '\0';
            break;
        }
        *cur_char = '\0';

        if (cur_char <= path)
        {
            break;
        }
    }

    return SUCCESS;
}