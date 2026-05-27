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

//Removes all the files storage data
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

//Read all the data in a file
//Buffer must be of size file->allocated_size * CHUNK_SIZE at least
StatusCode ws_file_read_all(
    Workspace *workspace,
    File *file,
    char *buffer,
    const size_t buffer_size
)
{
    if (!workspace || !file || !buffer)
    {
        return NULL_POINTER_PASSED;
    }

    if (buffer_size < file->allocated_size * CHUNK_SIZE)
    {
        return INSUFFICIENT_ARRAY_PASSED;
    }

    return file_read_at(
        file,
        workspace->storage_man,
        0, //offset = 0 since we are reading the whole file
        file->allocated_size * CHUNK_SIZE,
        buffer,
        buffer_size
    );
}


StatusCode ws_file_read_at(
    Workspace *workspace,
    File *file,
    const size_t offset,
    const size_t amount,
    char *buffer,
    const size_t buffer_size
)
{
    if (!workspace || !file || !buffer)
    {
        return NULL_POINTER_PASSED;
    }

    if (buffer_size < amount)
    {
        return INSUFFICIENT_ARRAY_PASSED;
    }

    //file_read_at validates offset and whether offset + amount is in bound
    return file_read_at(
        file,
        workspace->storage_man,
        offset,
        amount,
        buffer,
        buffer_size
    );
}