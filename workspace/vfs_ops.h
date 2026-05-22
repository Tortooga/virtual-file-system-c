#ifndef VFS_OPS_H
#define VFS_OPS_H

#include "status.h"
#include "workspace.h"

//file_name must be null terminated
StatusCode ws_create_file(Workspace *workspace, char *file_name);

//folder_name must be null terminated
StatusCode ws_create_folder(Workspace *workspace, char *folder_name);

#endif