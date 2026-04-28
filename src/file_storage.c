#include "../include/file_storage.h"

int get_available_chunk_extent_index(File *file);
StatusCode free_chunk_extent(ChunkExtent *chunk_extent, StorageMan *storage_man);
void chunk_extent_copy_range(ChunkExtent *target_chunk_extent, ChunkExtent *copy_chunk_extent);

//Assumes file.data_chunk_extents is compact except at empty_position
//shifts all non-empty chunk extents in file.data_chunk_extents to the left after a certain position.
//position supplied will be overwritten and should be empty
StatusCode chunk_extent_left_shift(ChunkExtent *empty_position, File *file)
{
    if (!empty_position || !file)
    {
        return NULL_POINTER_PASSED;
    }

    if (!empty_position->is_empty)
    {
        return INVALID_ARGUMENT;
    }

    //Upper limit of file.data_chunk_extents, extents before it are not guaranteed to be occupied
    //They are guaranteed to be initialised however.
    ChunkExtent *array_upper_lim = file->data_chunk_extents + MAX_FILE_CHUNK_EXTENTS_AMOUNT;
    
    //If supplied position is outside of the bounds of file->data_chunk_extents
    if (empty_position >= array_upper_lim || empty_position < file->data_chunk_extents)
    {
        return INVALID_ARGUMENT;
    }

    //if we are at the end of the array there is no need for left shift
    if (empty_position + 1 == array_upper_lim)
    {
        return SUCCESS;
    }

    //if we are at the end of the compact cluster no need for left shift
    if ((empty_position + 1)->is_empty)
    {
        return SUCCESS;
    }
    
    //since the last 2 early return conditions guarantee a condidate for this position
    empty_position->is_empty = false; 

    for (; empty_position < array_upper_lim; empty_position++)
    {
        //If we hit the end of the array or the end of the compact sequence we mark the last position as empty and halt
        if (empty_position + 1 >= array_upper_lim || (empty_position + 1)->is_empty)
        {
            empty_position->is_empty = true;
            break;
        }
        
        chunk_extent_copy_range(empty_position, empty_position + 1);
    }

    return SUCCESS;
}

void chunk_extent_copy_range(ChunkExtent *target_chunk_extent, ChunkExtent *copy_chunk_extent)
{
    //Both target and copy are guaranteed to have is_empty = false
    target_chunk_extent->chunk_amount = copy_chunk_extent->chunk_amount;
    target_chunk_extent->start = copy_chunk_extent->start;
}

StatusCode file_allocate_chunks(
    File *file, 
    StorageMan *storage_man, 
    size_t chunks_amount,
    ChunkExtent **out_chunk_extent)
{
    if (!file || !storage_man)
    {
        return NULL_POINTER_PASSED;
    }

    //searching for an available chunk_extent in file->data_chunk_extents
    int chunk_extent_index = get_available_chunk_extent_index(file);

    //if no extent was found
    if (chunk_extent_index == -1)
    {
        return FILE_CHUNK_EXTENT_LIMIT_EXCEEDED;
    }

    size_t first_chunk_index;
    //challoc validates chunks_amount
    StatusCode challoc_status_code = challoc(
        storage_man,
        chunks_amount,
        &first_chunk_index
    );

    //Error codes are global. They can be returned directly
    if (challoc_status_code != SUCCESS)
    {
        return challoc_status_code;
    }

    //marking chunk_extent as occupied
    file->data_chunk_extents[chunk_extent_index].is_empty = false;

    //challoc stores the first index of the allocated chunks onto the output parameter first_chunk_index
    file->data_chunk_extents[chunk_extent_index].start = first_chunk_index;

    //since challoc succeeded, we have allocated chunks with amount chunks_amount
    file->data_chunk_extents[chunk_extent_index].chunk_amount = chunks_amount;
    file->allocated_size += chunks_amount;

    //Placing the chunk extents memory address on the pointer passed as an output parameter
    *out_chunk_extent = &(file->data_chunk_extents[chunk_extent_index]);

    return SUCCESS;
}

StatusCode file_free_chunk_extent(File *file, ChunkExtent *chunk_extent, StorageMan *storage_man)
{
    if (!file || !chunk_extent || !storage_man)
    {
        return NULL_POINTER_PASSED;
    }

    if (chunk_extent->is_empty)
    {
        return INVALID_ARGUMENT;
    }

    StatusCode status = free_chunk_extent(chunk_extent, storage_man);

    if (status != SUCCESS)
    {
        return status;
    }

    chunk_extent->is_empty = true;
    file->allocated_size -= chunk_extent->chunk_amount;

    //To ensure compactness 
    status = chunk_extent_left_shift(chunk_extent, file);
    
    if (status != SUCCESS)
    {
        return status;
    }
    return SUCCESS;
}

//Partial failure causes partial deallocation
StatusCode free_chunk_extent(ChunkExtent *chunk_extent, StorageMan *storage_man)
{
    //protected helper function assumes validation from file_free_chunk_extent
    
    StatusCode status;
    for (int cur_pos = chunk_extent->start; cur_pos < chunk_extent->start + chunk_extent->chunk_amount; cur_pos++)
    {
        status = chfree(storage_man, cur_pos);
        if (status != SUCCESS)
        {
            return status;
        }
    }
    return SUCCESS;
}


//returns -1 if no available chunk_extent was found
int get_available_chunk_extent_index(File *file)
{
    //private helper only called in file_allocate_chunks which already validates file
    for (int i = 0; i < MAX_FILE_CHUNK_EXTENTS_AMOUNT; i++)
    {
        if (file->data_chunk_extents[i].is_empty)
        {
            return i;
        }
    }

    //no empty extent found
    return -1;
}