#ifndef VFS_OPS_H
#define VFS_OPS_H

#include "status.h"
#include "workspace.h"

//file_name must be null terminated
//creates file in CWD
StatusCode ws_create_file(Workspace *workspace, char *file_name);

//folder_name must be null terminated
//creates folder in CWD
StatusCode ws_create_folder(Workspace *workspace, char *folder_name);

//file_path must be null terminated
StatusCode ws_remove_file(Workspace *workspace, const char *file_path);


//folder_path must be null terminated
//recursive enables the recursive deletion of children
//attempting to delete a folder with children with recursive = false return error
StatusCode ws_remove_folder(Workspace *workspace, const char *folder_path, bool recursive);

//node_path and target_new_parent must be null terminated
StatusCode ws_move_node(Workspace *workspace, const char *node_path, const char *target_new_parent_path);
#endif