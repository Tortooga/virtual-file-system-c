#include "../include/files.h"

//Vacacny in Folder.sub_folder and Folder.sub_file is denoted by NULL pointers

typedef struct Folder {
    struct Folder *parent_folder;
    bool is_root;

    //One more position for enforced null terminator
    char name[MAX_NAME_LENGTH + 1]; 

    //Folder owns sub -files and -folders
    File *sub_files[MAX_SUB_FILES_AMOUNT];
    struct Folder *sub_folders[MAX_SUB_FOLDERS_AMOUNT];
} Folder;

StatusCode sub_folder_init(
    Folder *folder,
    const char *name,
    size_t name_length,
    Folder *parent_folder
);

StatusCode root_folder_init(Folder *folder);