#ifndef ENTRY_RELOCATION_H
#define ENTRY_RELOCATION_H

#include "folders.h"
#include "files.h"
#include "vfs_entry_store.h"

//renames file
//will fail if name is not unique or not available
//or if new name is equal to old name
StatusCode rename_file(
    File *file,
    char *name,
    const size_t name_length
);

//renames file
//will fail if name is not unique or not available
//or if new name is equal to old name
StatusCode rename_folder(
    Folder *folder,
    char *name,
    const size_t name_length
);

//Moves file to new parent 
//Ensures that file.name is unique under new parent
//Is not responsible for verifying that new_parent is a part of the same vfs as file.
StatusCode move_file(File *file, Folder *new_parent);

//Moves folder to new parent
//Ensures that folder.name is unique under new parent
//Ensures that new parent is not a descendant of folder
//Is not responsible for verifying that new_parent is a part of the same vfs as file.
StatusCode move_folder(Folder *folder, Folder *new_parent);
#endif