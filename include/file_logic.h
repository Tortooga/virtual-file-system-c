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

#endif