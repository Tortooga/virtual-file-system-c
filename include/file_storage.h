#ifndef FILE_STORAGE_H
#define FILE_STORAGE_H

#include "files.h"
#include "status.h"

StatusCode file_allocate_chunks(
    File *file, 
    StorageMan *storage_man, 
    size_t chunks_amount,
    ChunkExtent **out_chunk_extent);

StatusCode file_free_chunk_extent(
    File *file, 
    ChunkExtent *chunk_extent,
    StorageMan *storage_man);

#endif