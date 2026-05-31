#include "navigation.h"
#include "workspace.h"
#include "folders.h"
#include "path_utils.h"
#include "queries.h"
#include "status.h"
#include "settings.h"

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
    
    StatusCode status;
    VFSNode target_node;

    if (strcmp(path, BACK_TO_PARENT_OPERATOR) == 0)
    {
        if (workspace->cur_folder->is_root)
        {
            return INVALID_ROOT_OPERATION;
        }

        target_node.type = FOLDER_NODE;
        target_node.node.folder = workspace->cur_folder->parent_folder;

        status = get_node_path(
            &target_node,
            workspace->cur_path,
            MAX_PATH_NODES_AMOUNT * (MAX_NAME_LENGTH + 1) + 1
        );

        if (status != SUCCESS)
        {
            return status;
        }
        
        workspace->cur_folder = workspace->cur_folder->parent_folder;

        return SUCCESS;
    }


    //path validation handled by ws_resolve_path
    status = ws_resolve_path(
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
        return EXPECTED_FOLDER_GOT_FILE;
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

//if path is null target folder will be WD 
//path must be null terminated
StatusCode ws_get_sub_entries(
    Workspace *workspace,
    const char *path,
    VFSNode *node_buffer,
    const size_t node_buffer_length,
    size_t *out_nodes_amount
)
{
    if (!workspace || !node_buffer || !out_nodes_amount)
    {
        return NULL_POINTER_PASSED;
    }

    *out_nodes_amount = 0;

    Folder *target_folder = workspace->cur_folder;

    if (path)
    {
        VFSNode node;

        StatusCode status = ws_resolve_path(
            workspace,
            path,
            &node
        );

        if (status != SUCCESS)
        {
            return status;
        }

        if (node.type == FILE_NODE)
        {
            return EXPECTED_FOLDER_GOT_FILE;
        }
        target_folder = node.node.folder;
    }

    for (size_t i = 0; i < MAX_SUB_FILES_AMOUNT; i++)
    {
        if (target_folder->sub_files[i] == NULL)
        {
            continue;
        }
        if (*out_nodes_amount >= node_buffer_length)
        {
            return DATA_OVER_FLOW;
        }
        node_buffer[*out_nodes_amount].node.file = target_folder->sub_files[i];
        node_buffer[*out_nodes_amount].type = FILE_NODE;
        
        (*out_nodes_amount)++;
    }

    for (size_t i = 0; i < MAX_SUB_FOLDERS_AMOUNT; i++)
    {
        if (target_folder->sub_folders[i] == NULL)
        {
            continue;
        }

        if (*out_nodes_amount >= node_buffer_length)
        {
            return DATA_OVER_FLOW;
        }

        node_buffer[*out_nodes_amount].node.folder = target_folder->sub_folders[i];
        node_buffer[*out_nodes_amount].type = FOLDER_NODE;

        (*out_nodes_amount)++;
    }

    return SUCCESS;
}