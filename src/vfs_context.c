#include "../include/vfs_context.h"
#include "../include/files.h"
#include "../include/folders.h"
#include "../include/path_utils.h"

void root_path_init(char *out_path);

StatusCode vfs_context_init(
    VFSEntryStore *entry_store,
    VFSContext *out_vfs_context
)
{
    if (!entry_store || !out_vfs_context)
    {
        return NULL_POINTER_PASSED;
    }


    out_vfs_context->cur_folder = &entry_store->root;
    out_vfs_context->safe_mode = false;
    out_vfs_context->entry_store = entry_store;

    //Calling this helper is more efficient than calling the all-purpose get_node_path 
    root_path_init(out_vfs_context->cur_path);

    return SUCCESS;
}

void root_path_init(char *out_path)
{
    out_path[0] = PATH_DELIMITER;
    out_path[1] = '\0';
}