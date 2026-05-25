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


//Marks chunks as free in storage_man.allocation_map
//Removes extent from file
//left shifts file.chunk_extents to enforce compactness
StatusCode file_free_chunk_extent(
    File *file, 
    ChunkExtent *chunk_extent,
    StorageMan *storage_man);

//Writes data into chunk enforcing file ownership
StatusCode file_write_chunk(
    File *file, 
    StorageMan *storage_man, 
    size_t chunk_pos,
    char *data,
    size_t data_length);

//copies data from chunk into buffer enforcing file ownership
StatusCode file_read_chunk(
    File *file, 
    StorageMan *storage_man,
    size_t chunk_pos,
    char *out_data,
    size_t data_length);

//Assumes file.data_chunk_extents is compact.
//Shifts all chunks beyond and including the specified position to the right by shift_amount
//Positions shifted from must immediately be filled otherwise compactness will be broken
StatusCode chunk_extent_right_shift(
    ChunkExtent *position, 
    size_t shift_amount, 
    File *file
);

#endif