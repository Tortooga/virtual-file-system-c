#include "vfs_entry_store.h"
#include "workspace.h"
#include "status.h"

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