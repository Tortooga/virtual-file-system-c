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
    bool safe_mode; //whether the VFS should be in safe mode(a state in which deleting is restricted)
    Folder *cur_folder;
    VFSEntryStore *entry_store;
    //this is enough to store the names of MAX_PATH_NODES_AMOUNT path nodes with the maximum name length along with their path delimiters and null terminator
    char cur_path[MAX_PATH_NODES_AMOUNT * (MAX_NAME_LENGTH + 1) + 1]; 
} Workspace;


StatusCode workspace_init(
    VFSEntryStore *entry_store,
    Workspace *out_workspace
);
#endif