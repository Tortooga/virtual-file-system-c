#include "workspace.h"
#include "files.h"
#include "folders.h"
#include "path_utils.h"

void root_path_init(char *out_path);

StatusCode workspace_init(
    VFSEntryStore *entry_store,
    StorageMan *storage_man,
    Workspace *out_workspace
)
{
    if (!entry_store || !out_workspace || !storage_man)
    {
        return NULL_POINTER_PASSED;
    }


    out_workspace->cur_folder = &entry_store->root;
    out_workspace->safe_mode = false;
    out_workspace->entry_store = entry_store;
    out_workspace->storage_man = storage_man;
    
    //Calling this helper is more efficient than calling the all-purpose get_node_path 
    root_path_init(out_workspace->cur_path);

    return SUCCESS;
}

void root_path_init(char *out_path)
{
    out_path[0] = PATH_DELIMITER;
    out_path[1] = '\0';
}