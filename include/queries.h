#include "folders.h"

StatusCode search_sub_folder(
    Folder *current_folder, 
    char *target_folder_name, 
    size_t target_folder_name_length, 
    Folder **out_result_folder_addrs);

StatusCode search_sub_file(
    Folder *current_folder,
    const char *target_file_name, 
    size_t target_file_name_length, 
    File **out_result_file_addrs);