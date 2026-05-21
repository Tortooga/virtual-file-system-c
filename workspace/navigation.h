#include "workspace.h"
#include "folders.h"
#include "status.h"

//resolve path with wp context
//path must be null terminated
StatusCode wp_resolve_path(Workspace *workspace, const char *path, VFSNode *out_node);