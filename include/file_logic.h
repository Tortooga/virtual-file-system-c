#ifndef FILE_LOGIC_H
#define FILE_LOGIC_H

#include "status.h"
#include "files.h"
#include "storage.h"

StatusCode file_append(
    File *file, 
    StorageMan *storage_man,
    char* data,
    size_t data_length);

StatusCode file_read_at(
    File *file,
    StorageMan *storage_man,
    size_t offset,
    size_t amount,
    char *buffer,
    size_t buffer_size
);
#endif