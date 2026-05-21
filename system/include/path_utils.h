#ifndef PATH_UTILS_H
#define PATH_UTILS_H
#include "status.h"

#include <stddef.h>



//Parses path into nodes
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
);


//node_name is null terminated(enforced in all entry initialisers)
//path_end points to the end of the path(at the null terminator), where we are supposed to start the appended name
//it is important that it points at the null terminator so that it could be overwritten
//available_bytes_amount is the amount of available bytes after and not including null terminator
StatusCode append_node_name_to_path(
    const char *node_name, 
    char *path_end, 
    const size_t available_bytes_amount);

//node name guaranteed to be null terminated 
//out_path will be null terminated
StatusCode get_node_path(VFSNode *node, char *out_path, size_t path_length);
#endif