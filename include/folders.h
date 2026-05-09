#ifndef FOLDERS_H
#define FOLDERS_H

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


typedef enum
{
    FILE_NODE,
    FOLDER_NODE,
} NodeType;

//Is not the framwork for the VFS heirarchy
//Only used to mimic polymophism in search functions
//After which it is resolved immediately
typedef struct 
{
    NodeType type;
    union 
    {
        File *file;
        Folder *folder;
    } node;
} VFSNode;



//Initialises output param folder into a sub folder of parent_folder
//Name not assumed (but is allowed) to be C-string, NULL termination is enforced within the function.
//Enforces name uniqueness
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

//Compare the name of an initialised file/folder(where null terminator is enforced) to the name of a file/folder where it isnt
StatusCode name_eq(const char *initialised_name, const char *uninitialised_name, size_t uninitialised_name_length, bool *out_result);


#endif