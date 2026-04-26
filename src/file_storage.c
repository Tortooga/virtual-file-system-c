#include "../include/file_storage.h"

int get_available_chunk_extent_index(File *file);

StatusCode file_allocate_chunks(
    File *file, 
    StorageMan *storage_man, 
    size_t chunks_amount,
    size_t *out_first_chunk_index)
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

    //challoc validates chunks_amount and out_first_chunk_index
    StatusCode challoc_status_code = challoc(
        storage_man,
        chunks_amount,
        out_first_chunk_index
    );

    //Error codes are global. They can be returned directly
    if (challoc_status_code != SUCCESS)
    {
        return challoc_status_code;
    }

    //marking chunk_extent as occupied
    file->data_chunk_extents[chunk_extent_index].is_empty = false;

    //challoc stores the first index of the allocated chunks onto the output parameter out_first_chunk_index
    file->data_chunk_extents[chunk_extent_index].start = *out_first_chunk_index;

    //since challoc succeeded, we have allocated chunks with amount chunks_amount
    file->data_chunk_extents[chunk_extent_index].chunk_amount = chunks_amount;
    file->allocated_size += chunks_amount;

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