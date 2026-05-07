#include "folders.h"

StatusCode search_sub_folder(
    Folder *current_folder, 
    char *target_folder_name, 
    size_t target_folder_name_length, 
    Folder **out_result_folder_addrs);