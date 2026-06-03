#ifndef FILE_IO_H
#define FILE_IO_H

#include "status.h"

#include "workspace.h"

//target_file_path must be null terminated
StatusCode ws_file_append(
    Workspace *workspace,
    const char *target_file_path,
    char *data,
    const size_t data_length
);

//Removes all the files storage data
StatusCode ws_file_clear(
    Workspace *workspace,
    char *target_file_path
);

//Read all the data in a file
//Buffer must be of size file->allocated_size * CHUNK_SIZE at least
StatusCode ws_file_read_all(
    Workspace *workspace,
    char *target_file_path,
    char *buffer,
    const size_t buffer_size,
    size_t *out_data_amount
);

StatusCode ws_file_read_at(
    Workspace *workspace,
    char *target_file_path,
    const size_t offset,
    const size_t amount,
    char *buffer,
    const size_t buffer_size
);

#endif