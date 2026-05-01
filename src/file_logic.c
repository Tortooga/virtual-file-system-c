#include "../include/file_logic.h"
#include "../include/files.h"
#include "../include/file_storage.h"
#include "../include/status.h"

#include <stdio.h>

StatusCode get_remaining_data(char *out_remaining_data, char *data_start, size_t data_amount);
bool uses_full_chunks(size_t data_length);
size_t get_chunks_amount(size_t data_length);

//Partial Failure leads rollback deallocation of allocated chunks
StatusCode file_append(
    File *file, 
    StorageMan *storage_man,
    char* data,
    size_t data_length)
{
    if (!file || !storage_man || !data)
    {
        return NULL_POINTER_PASSED;
    }

    if (data_length > (storage_man->free_chunk_count * CHUNK_SIZE))
    {
        return DATA_OVER_FLOW;
    }

    if (data_length == 0)
    {
        return INVALID_OPERATION;
    }

    //guaranteed to be greater than 0
    size_t chunks_needed = get_chunks_amount(data_length);
    ChunkExtent *allocated_chunk_extent;
    
    StatusCode status = file_allocate_chunks(
        file,
        storage_man,
        chunks_needed,
        &allocated_chunk_extent
    );

    if (status != SUCCESS)
    {
        return status;
    }
    
    // allocated_chunk_extent->amount is guaranteed to be the same as chunks_needed
    // since the only way uses_full_chunks returns 0 is if the last chunk is not going to be filled, we subtract one from chunks needed if that happens
    size_t full_chunks_upper_lim = allocated_chunk_extent->start + (chunks_needed - (size_t)(!uses_full_chunks(data_length))); 
    size_t cur_chunk = allocated_chunk_extent->start;
    char *cur_data_addrs = data;
    for (; cur_chunk < full_chunks_upper_lim; cur_chunk++)
    {
        // any chunk addressed in this block is guaranteed to be filled
        status = file_write_chunk(
            file,
            storage_man,
            cur_chunk,
            cur_data_addrs,
            CHUNK_SIZE 
        );
        
        if (status != SUCCESS)
        {
            file_free_chunk_extent(file, allocated_chunk_extent, storage_man);
            return status; 
        }
        
        cur_data_addrs += CHUNK_SIZE;
    }
    
    if (uses_full_chunks(data_length))
    {
        return SUCCESS;
    }
    
    //handling partially filled final chunk
    char remaining_data[CHUNK_SIZE];
    get_remaining_data(
        remaining_data, 
        cur_data_addrs, 
        data_length % CHUNK_SIZE);

    status = file_write_chunk(
        file,
        storage_man,
        cur_chunk,
        remaining_data,
        CHUNK_SIZE
    );
    if (status != SUCCESS)
    {
        file_free_chunk_extent(file, allocated_chunk_extent, storage_man);
    }
    return status;
}

StatusCode get_remaining_data(char *out_remaining_data, char *data_start, size_t data_amount)
{
    //private helper. Only call from file_append
    //Caller guarantees out_remaining_data is of length >= CHUNK_SIZE
    if (!out_remaining_data || !data_start)
    {
        return NULL_POINTER_PASSED;
    }

    int cur_byte_index = 0;
    for (; cur_byte_index < data_amount; cur_byte_index++)
    {
        out_remaining_data[cur_byte_index] = data_start[cur_byte_index];
    }

    for (; cur_byte_index < CHUNK_SIZE; cur_byte_index++)
    {
        out_remaining_data[cur_byte_index] = '\0';
    }

    return SUCCESS;
}

bool uses_full_chunks(size_t data_length)
{
    //is able to fill all the required chunks 
    return data_length % CHUNK_SIZE == 0; 
}

//maximum amount of chunks needed to accomidate for data of size data_length bytes 
size_t get_chunks_amount(size_t data_length)
{
    if (uses_full_chunks(data_length))
    {
        return data_length / CHUNK_SIZE;
    }

    return (data_length / CHUNK_SIZE) + 1;
}