#include "files.h"
#include "file_logic.h"

#include "workspace.h"
#include "navigation.h"

#include "status.h"


//target_file_path must be null terminated
StatusCode ws_file_append(
    Workspace *workspace,
    const char *target_file_path,
    char *data,
    const size_t data_length
)
{
    if (!workspace || !target_file_path || !data)
    {
        return NULL_POINTER_PASSED;
    }

    VFSNode target_file_node;
    StatusCode status = ws_resolve_path(
        workspace,
        target_file_path,
        &target_file_node
    );

    if (status != SUCCESS)
    {
        return status;
    }

    if (target_file_node.type != FILE_NODE)
    {
        return EXPECTED_FILE_GOT_FOLDER;
    }

    return file_append(
        target_file_node.node.file,
        workspace->storage_man,
        data,
        data_length
    );
}

StatusCode ws_file_clear(
    Workspace *workspace,
    File *file
)
{
    if (!workspace || !file)
    {
        return NULL_POINTER_PASSED;
    }

    return file_delete_data(
        file,
        workspace->storage_man
    );
}