#include "navigation.h"
#include "workspace.h"
#include "folders.h"
#include "path_utils.h"
#include "queries.h"
#include "status.h"

#include <string.h>

//resolve path with wp context
//path must be null terminated
//absolute paths start with '/'. While relative paths do not
StatusCode ws_resolve_path(Workspace *workspace, const char *path, VFSNode *out_node)
{
    if (!workspace || !path || !out_node)
    {
        return NULL_POINTER_PASSED;
    }

    size_t path_length = strlen(path); 

    if (path_length == 0)
    {
        out_node->type = FOLDER_NODE;
        out_node->node.folder = &workspace->entry_store->root;

        return SUCCESS;
    }

    //relative resolution is default
    Folder *root_resolution_folder = workspace->cur_folder;

    if (path[0] == PATH_DELIMITER)
    {
        if (path_length == 1)
        {
            out_node->type = FOLDER_NODE;
            out_node->node.folder = &workspace->entry_store->root;

            return SUCCESS;
        }

        //path starting with "/" causes absolute resolution
        root_resolution_folder = &workspace->entry_store->root;
        path++;
        path_length--;
    }

    return resolve_path(
        path,
        path_length,
        root_resolution_folder,
        out_node
    );
}

//Path must be null terminated
//Changes workspace.cur_folder to the folder at path
//Changes workspace.cur_path to the path of the new folder
StatusCode ws_change_cur_folder(Workspace *workspace, const char *path)
{
    if (!workspace || !path)
    {
        return NULL_POINTER_PASSED;
    }

    VFSNode target_node;

    //path validation handled by ws_resolve_path
    StatusCode status = ws_resolve_path(
        workspace,
        path,
        &target_node
    );

    if (status != SUCCESS)
    {
        return status;
    }

    if (target_node.type != FOLDER_NODE)
    {
        return ATTEMPTED_TO_CD_INTO_FILE;
    }

    //We cannot directly assign workspace->cur_path the path
    //As path may be relative or might not be well formatted
    status = get_node_path(
        &target_node,
        workspace->cur_path,
        MAX_PATH_NODES_AMOUNT * (MAX_NAME_LENGTH + 1) + 1  //length of cur_path
    );

    if (status != SUCCESS)
    {
        return status;
    }

    workspace->cur_folder = target_node.node.folder;
    
    return SUCCESS;
}