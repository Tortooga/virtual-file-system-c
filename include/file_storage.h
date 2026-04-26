#ifndef FILE_STORAGE_H
#define FILE_STORAGE_H

#include "files.h"
#include "status.h"

StatusCode file_allocate_chunks(
    File *file, 
    StorageMan *storage_man, 
    size_t chunks_amount,
    size_t *out_first_chunk_index);

#endif