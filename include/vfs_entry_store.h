#ifndef VFS_ENTRY_STORE_H
#define VFS_ENTRY_STORE_H

#include "settings.h"
#include "files.h"
#include "folders.h"

#include <stdbool.h>

/*
Owns all addressable VFS entries.
Does not enforce any structure or hierarchy. (Hierarchy is implicitly encoded within the nodes themselves)
It, however, acts as an index for all the entries.

Vacancy in any file/folder position is indicated by the allocation maps */
typedef struct 
{
    File files[VFS_MAX_FILES_AMOUNT];
    Folder folders[VFS_MAX_FOLDERS_AMOUNT];

    //Allocation maps indicate whether a position in the entry arrays is vacant or not.
    //Refer to design documentation to see why it is like that.
    bool files_allocation_map[VFS_MAX_FILES_AMOUNT];
    bool folders_allocation_map[VFS_MAX_FOLDERS_AMOUNT];

    Folder root; 
} VFSEntryStore;

StatusCode vfs_entry_store_init(VFSEntryStore *out_vfs_entry_store);

#endif