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

#endif