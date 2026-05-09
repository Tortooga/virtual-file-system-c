#ifndef PATH_PARSER_H
#define PATH_PARSER_H
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

#endif