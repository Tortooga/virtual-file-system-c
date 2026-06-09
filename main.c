#include "storage.h"
#include "settings.h"
#include "vfs_entry_store.h"

#include "workspace.h"

#include "cli_runtime.h"

#include <stdlib.h>

int main(void)
{
    StorageMan storage_man = {0};
    VFSEntryStore entry_store = {0};
    Workspace workspace = {0};
    
    int exit_code = EXIT_SUCCESS;

    StatusCode status = storage_man_create(&storage_man);
    
    if (status != SUCCESS)
    {
        exit_code = EXIT_FAILURE;
        goto cleanup;
    }
    
    
    status = vfs_entry_store_init(&entry_store);
    
    if (status != SUCCESS)
    {
        exit_code = EXIT_FAILURE;
        goto cleanup;
    }
    
    
    status = workspace_init(
        &entry_store,
        &storage_man,
        &workspace
    );
    
    if (status != SUCCESS)
    {
        exit_code = EXIT_FAILURE;
        goto cleanup;
    }
    
    status = cli_run(
        &storage_man,
        &entry_store,
        &workspace
    );
    
    if (status != SUCCESS)
    {
        exit_code = EXIT_FAILURE;
    }

    cleanup:
        workspace_destroy(&workspace);
        vfs_entry_store_destroy(&entry_store);
        storage_man_destroy(&storage_man);
        
        return exit_code;
} 


