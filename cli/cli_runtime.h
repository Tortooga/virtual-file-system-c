#ifndef CLI_RUNTIME_H
#define CLI_RUNTIME_H

#include "status.h"
#include "vfs_entry_store.h"

#include "workspace.h"

#define OUTPUT_BUFFER_SIZE 1024



StatusCode cli_run(
    StorageMan *storage_man, 
    VFSEntryStore *entry_store, 
    Workspace *workspace
);

#endif