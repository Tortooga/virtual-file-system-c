#ifndef workspace_H
#define workspace_H

#include "folders.h"
#include "vfs_entry_store.h"

#include <stdbool.h> 



/*
manages the current state of the file system.
safemode defaults to false
*/
typedef struct
{
    //whether the VFS should be in safe mode(a state in which deleting is restricted)
    bool safe_mode; 
    
    Folder *cur_folder;
    
    VFSEntryStore *entry_store;
    //A dependancy of VFS
    StorageMan *storage_man; 
    //this is enough to store the names of MAX_PATH_NODES_AMOUNT path nodes with the maximum name length along with their path delimiters and null terminator
    char cur_path[MAX_PATH_NODES_AMOUNT * (MAX_NAME_LENGTH + 1) + 1]; 
} Workspace;


StatusCode workspace_init(
    VFSEntryStore *entry_store,
    StorageMan *storage_man_init,
    Workspace *out_workspace
);
#endif