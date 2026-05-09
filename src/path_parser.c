#include "../include/files.h"
#include "../include/folders.h"
#include "../include/status.h"


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