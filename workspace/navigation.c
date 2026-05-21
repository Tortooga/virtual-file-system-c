#include "navigation.h"
#include "workspace.h"
#include "folders.h"
#include "queries.h"
#include "status.h"

#include <string.h>

//resolve path with wp context
//path must be null terminated
//absolute paths start with '/'. While relative paths do not
StatusCode wp_resolve_path(Workspace *workspace, const char *path, VFSNode *out_node)
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

StatusCode change_cur_folder(Workspace *workspace, char *path)
{
    
}