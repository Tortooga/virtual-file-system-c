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
    char *target_file_path
)
{
    if (!workspace || !target_file_path)
    {
        return NULL_POINTER_PASSED;
    }

    VFSNode node;
    StatusCode status = ws_resolve_path(
        workspace,
        target_file_path,
        &node
    );

    if (status != SUCCESS)
    {
        return status;
    }

    if (node.type != FILE_NODE)
    {
        return EXPECTED_FILE_GOT_FOLDER;
    }

    return file_delete_data(
        node.node.file,
        workspace->storage_man
    );
}

//Read all the data in a file
//Buffer must be of size file->allocated_size * CHUNK_SIZE at least
StatusCode ws_file_read_all(
    Workspace *workspace,
    char *target_file_path,
    char *buffer,
    const size_t buffer_size,
    size_t *out_data_amount
)
{
    if (!workspace || !target_file_path || !buffer || !out_data_amount)
    {
        return NULL_POINTER_PASSED;
    }

    *out_data_amount = 0;
    
    VFSNode node;
    StatusCode status = ws_resolve_path(
        workspace,
        target_file_path,
        &node
    );
    
    if (status != SUCCESS)
    {
        return status;
    }
    
    if (node.type != FILE_NODE)
    {
        return EXPECTED_FILE_GOT_FOLDER;
    }
    
    if (buffer_size < node.node.file->allocated_size * CHUNK_SIZE)
    {
        return INSUFFICIENT_ARRAY_PASSED;
    }

    if (node.node.file->allocated_size == 0)
    {
        return SUCCESS;
    }

    status = file_read_at(
        node.node.file,
        workspace->storage_man,
        0, //offset = 0 since we are reading the whole file
        node.node.file->allocated_size * CHUNK_SIZE,
        buffer,
        buffer_size
    );

    if (status != SUCCESS)
    {
        return status;
    }

    *out_data_amount = node.node.file->allocated_size * CHUNK_SIZE;

    return SUCCESS;
}


StatusCode ws_file_read_at(
    Workspace *workspace,
    char *target_file_path,
    const size_t offset,
    const size_t amount,
    char *buffer,
    const size_t buffer_size
)
{
    if (!workspace || !target_file_path || !buffer)
    {
        return NULL_POINTER_PASSED;
    }

    if (buffer_size < amount)
    {
        return INSUFFICIENT_ARRAY_PASSED;
    }

    VFSNode node;

    StatusCode status = ws_resolve_path(
        workspace,
        target_file_path,
        &node
    );

    if (status != SUCCESS)
    {
        return status;
    }

    if (node.type != FILE_NODE)
    {
        return EXPECTED_FILE_GOT_FOLDER;
    }

    //file_read_at validates offset and whether offset + amount is in bound
    return file_read_at(
        node.node.file,
        workspace->storage_man,
        offset,
        amount,
        buffer,
        buffer_size
    );
}