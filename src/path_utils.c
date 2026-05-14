#include "../include/files.h"
#include "../include/folders.h"
#include "../include/status.h"

#include <string.h>


StatusCode parse_node(
    const char **cur_path_char,
    const char *path_upper_lim,

    const size_t node_name_buffer_length,
    char *out_node_name_buffer
);

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