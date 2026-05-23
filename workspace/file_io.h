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

#endif