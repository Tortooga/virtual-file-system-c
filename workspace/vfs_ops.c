#include "vfs_entry_store.h"
#include "status.h"
#include "folders.h"

#include "workspace.h"
#include "navigation.h"

#include <string.h>

//file_name must be null terminated
StatusCode ws_create_file(Workspace *workspace, char *file_name)
{
    if (!workspace || !file_name)
    {
        return NULL_POINTER_PASSED;
    }

    //vfs_sub_file_init validates file_name
    return vfs_sub_file_init(
        NULL,
        workspace->entry_store,
        file_name,
        strlen(file_name),
        workspace->cur_folder
    );
}

//folder_name must be null terminated
StatusCode ws_create_folder(Workspace *workspace, char *folder_name)
{
    if (!workspace || !folder_name)
    {
        return NULL_POINTER_PASSED;
    }

    //vfs_sub_folder_init validates folder_name
    return vfs_sub_folder_init(
        NULL,
        workspace->entry_store,
        folder_name,
        strlen(folder_name),
        workspace->cur_folder
    );
}

//file_path must be null terminated
StatusCode ws_remove_file(Workspace *workspace, const char *file_path)
{
    if (!workspace || !file_path)
    {
        return NULL_POINTER_PASSED;
    }

    if (workspace->safe_mode)
    {
        return INVALID_SAFE_MODE_OP;
    }

    VFSNode path_node;

    StatusCode status = ws_resolve_path(
        workspace,
        file_path,
        &path_node
    );

    if (status != SUCCESS)
    {
        return status;
    }

    if (path_node.type != FILE_NODE)
    {
        return ATTEMPTED_TO_RM_FOLDER;
    }

    return delete_vfs_file(
        workspace->entry_store,
        path_node.node.file,
        workspace->storage_man
    );
}
