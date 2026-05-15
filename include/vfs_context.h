#ifndef VFS_CONTEXT_H
#define VFS_CONTEXT_H

#include "files.h"
#include "folders.h"
#include "vfs_entry_store.h"

#include <stdbool.h> 



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

#endif