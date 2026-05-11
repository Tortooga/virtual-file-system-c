#include "../include/vfs_context.h"
#include "../include/files.h"
#include "../include/folders.h"

StatusCode vfs_entry_store_init(VFSEntryStore *out_vfs_entry_store)
{
    if (!out_vfs_entry_store)
    {
        return NULL_POINTER_PASSED;
    }

    for (size_t i = 0; i < VFS_MAX_FILES_AMOUNT; i++)
    {
        out_vfs_entry_store->files_allocation_map[i] = false;
    }

    for (size_t i = 0; i < VFS_MAX_FOLDERS_AMOUNT; i++)
    {
        out_vfs_entry_store->folders_allocation_map[i] = false;
    }

    StatusCode status = root_folder_init(&out_vfs_entry_store->root);

    
    return status;
}

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