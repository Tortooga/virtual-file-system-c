#include "workspace.h"
#include "folders.h"
#include "status.h"

//resolve path with wp context
//path must be null terminated
StatusCode ws_resolve_path(Workspace *workspace, const char *path, VFSNode *out_node);

//Path must be null terminated
//Changes workspace.cur_path to the folder at path
StatusCode change_cur_folder(Workspace *workspace, const char *path);

//Path must be null terminated
//Changes workspace.cur_folder to the folder at path
//Changes workspace.cur_path to the path of the new folder
StatusCode ws_change_cur_folder(Workspace *workspace, const char *path);


//if path is null target folder will be WD 
//path must be null terminated
StatusCode ws_get_sub_entries(
    Workspace *workspace,
    const char *path,
    VFSNode *node_buffer,
    size_t node_buffer_length,
    size_t *out_nodes_amount
);