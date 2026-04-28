#ifndef FILE_STORAGE_H
#define FILE_STORAGE_H

#include "files.h"
#include "status.h"

//Allocates chunks to file and records chunk extent
StatusCode file_allocate_chunks(
    File *file, 
    StorageMan *storage_man, 
    size_t chunks_amount,
    ChunkExtent **out_chunk_extent);


//Frees a chunk extent
StatusCode file_free_chunk_extent(
    File *file, 
    ChunkExtent *chunk_extent,
    StorageMan *storage_man);

StatusCode chunk_extent_left_shift(ChunkExtent *empty_position, File *file);
#endif