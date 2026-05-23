#include "vfs_entry_store.h"
#include "status.h"
#include "folders.h"
#include "entry_relocation.h"

#include "workspace.h"
#include "navigation.h"

#include <string.h>
#include <stdbool.h>

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
        return EXPECTED_FILE_GOT_FOLDER;
    }

    return delete_vfs_file(
        workspace->entry_store,
        path_node.node.file,
        workspace->storage_man
    );
}

//folder_path must be null terminated
//recursive enables the recursive deletion of children
//attempting to delete a folder with children with recursive = false return error
StatusCode ws_remove_folder(Workspace *workspace, const char *folder_path, bool recursive)
{
    if (!workspace || !folder_path)
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
        folder_path,
        &path_node
    );

    if (status != SUCCESS)
    {
        return status;
    }

    if (path_node.type != FOLDER_NODE)
    {
        return EXPECTED_FILE_GOT_FOLDER;
    } 

    //delete_vfs_folder handles recursive deleting and validation of folder
    return delete_vfs_folder(
        path_node.node.folder,
        workspace->entry_store,
        workspace->storage_man,
        recursive
    );
}

//node_path and target_new_parent must be null terminated
StatusCode ws_move_node(Workspace *workspace, const char *node_path, const char *target_new_parent_path)
{
    if (!workspace || !node_path || !target_new_parent_path)
    {
        return NULL_POINTER_PASSED;
    }

    VFSNode node;
    StatusCode status = ws_resolve_path(
        workspace, node_path, &node
    );

    if (status != SUCCESS)
    {
        return status;
    }

    VFSNode target_new_parent_node;
    status = ws_resolve_path(
        workspace,
        target_new_parent_path,
        &target_new_parent_node
    );

    if (status != SUCCESS)
    {
        return status;
    }

    if (target_new_parent_node.type != FOLDER_NODE)
    {
        return EXPECTED_FOLDER_GOT_FILE;
    }

    if (node.type == FILE_NODE)
    {
        status = move_file(
            node.node.file,
            target_new_parent_node.node.folder
        );
    }
    else
    {
        status = move_folder(
            node.node.folder,
            target_new_parent_node.node.folder
        );
    }

    return status;
}