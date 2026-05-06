#include "../include/files.h"

//Invariants
//a sub-entry is vacant if and only if it is NULL
//is_root = true if and only if parent_folder = NULL (See duplicate root denotion trade off in design.md)
//name is a C-string
typedef struct Folder {
    struct Folder *parent_folder;
    bool is_root;

    //One more position to enforce null termination
    char name[MAX_NAME_LENGTH + 1]; 

    //Folder owns sub -files and -folders
    File *sub_files[MAX_SUB_FILES_AMOUNT];
    struct Folder *sub_folders[MAX_SUB_FOLDERS_AMOUNT];
} Folder;



//Initialises output param folder into a sub folder of parent_folder
//Name not assumed (but is allowed) to be C-string, NULL termination is enforced within the function.
//Sub-entries initialised to NULL
StatusCode sub_folder_init(
    Folder *folder,
    const char *name,
    size_t name_length,
    Folder *parent_folder
);

//The only way to initialise a root folder
//Parent is set to NULL
StatusCode root_folder_init(Folder *folder);