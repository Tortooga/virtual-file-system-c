#include "../include/vfs_context.h"
#include "../include/files.h"
#include "../include/folders.h"

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

    return SUCCESS;
}



