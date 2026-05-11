#ifndef VFS_CONTEXT_H
#define VFS_CONTEXT_H

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

/*
manages the current state of the file system.
safemode defaults to false
*/
typedef struct
{
    bool safe_mode; //whether the VFS should be in safe mode(a state in which deleting is restricted)
    Folder *cur_folder;
    VFSEntryStore *entry_store;
} VFSContext;


StatusCode vfs_context_init(
    VFSEntryStore *entry_store,
    VFSContext *out_vfs_context
);

StatusCode vfs_entry_store_init(VFSEntryStore *out_vfs_entry_store);
#endif