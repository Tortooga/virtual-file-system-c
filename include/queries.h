#ifndef QUERIES_H
#define QUERIES_H

#include "folders.h"

/*
searchs for sub folder in current_folder by name
target_folder_name not assumed to be null terminated
if null termination in the name is encountered the name terminates
returns address of the folder if found onto out_result_folder_addrs
if the search fails out_result_folder_addrs is set to NULL
*/
StatusCode search_sub_folder(
    const Folder *current_folder, 
    const char *target_folder_name, 
    const size_t target_folder_name_length, 
    Folder **out_result_folder_addrs);

/*
searchs for sub file in current_folder by name
target_file_name not assumed to be null terminated
if null termination in the name is encountered the name terminates
returns address of the file if found onto out_result_file_addrs
if the search fails out_result_file_addrs is set to NULL
*/
StatusCode search_sub_file(
    const Folder *current_folder,
    const char *target_file_name, 
    const size_t target_file_name_length, 
    File **out_result_file_addrs);

/*
Resolves path into a node containing either a file or a folder(see VFSNode definition)

path always starts at (but does not include) the root folder
Does not assume or enforce path name NULL termination 
Failed search does not initialise out_node

Passing any folder in place of the root folder causes the resolution to start at the passed folder
this operation is safe
*/    
StatusCode resolve_path(
    const char *path, 
    const size_t path_length,
    Folder *root_folder, 
    VFSNode *out_node);

StatusCode global_search_nodes_by_name(
    const char *name,
    const size_t name_length,

    VFSEntryStore *entry_store,

    VFSNode *out_nodes,
    size_t nodes_buffer_length,

    size_t *out_nodes_amount
);
#endif