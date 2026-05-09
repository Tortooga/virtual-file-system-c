#include "folders.h"

StatusCode search_sub_folder(
    const Folder *current_folder, 
    const char *target_folder_name, 
    const size_t target_folder_name_length, 
    Folder **out_result_folder_addrs);

StatusCode search_sub_file(
    const Folder *current_folder,
    const char *target_file_name, 
    const size_t target_file_name_length, 
    File **out_result_file_addrs);

//path always starts at (but does not include) the root folder
//returns a node containing either a file or a folder(see VFSNode definition)
//Does not assume or enforce path name NULL termination 
//Failed search does not initialise out_node    
StatusCode resolve_path(
    const char *path, 
    const size_t path_length,
    Folder *root_folder, 
    VFSNode *out_node);