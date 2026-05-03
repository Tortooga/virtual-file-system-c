#include "../include/file_logic.h"
#include "../include/files.h"
#include "../include/file_storage.h"
#include "../include/status.h"

#include <stdio.h>

StatusCode get_remaining_data(char *out_remaining_data, char *data_start, size_t data_amount);
bool uses_full_chunks(size_t data_length);
size_t get_chunks_amount(size_t data_length);
StatusCode get_chunk_index(File *file, size_t relative_chunk_position, size_t *out_chunk_index);

StatusCode read_chunk_from_until(
    StorageMan *storage_man,
    File *file, //necessary to enforce ownership
    size_t start_offset,
    size_t amount,
    size_t relative_chunk_index,
    char **buffer
);

StatusCode read_full_chunks_and_update_buffer_pointer(
    File *file, 
    size_t relative_chunk_index,
    StorageMan *storage_man,
    char **buffer,
    size_t chunks_amount);

StatusCode file_read_at(
    File *file,
    StorageMan *storage_man,
    size_t offset,
    size_t amount,
    char *buffer,
    size_t buffer_size)
{
    if (!file || !storage_man || !buffer)
    {
        return NULL_POINTER_PASSED;
    }

    if (offset + amount > file->allocated_size * CHUNK_SIZE)
    {
        return INDEX_OUT_OF_BOUNDS;
    }

    if (amount == 0)
    {
        return INVALID_OPERATION;
    }
    
    if (buffer_size < amount)
    {
        return INSUFFICIENT_ARRAY_PASSED;
    }

    //ALL CHUNK INDICES ADDRESSED BELOW ARE RELATIVE

    //amount by which the offset is away from the first chunk start
    const size_t first_partial_chunk_offset = offset % CHUNK_SIZE;
    const size_t first_partial_chunk_index = offset / CHUNK_SIZE;
    char *cur_buffer_addrs = buffer;

    /*the first partial chunk is the chunk containing the offset
    it is the first chunk we proccess. Most of the time we will only processes the latter part of it after and including offset
    in the case that the offset starts where a chunk starts the first partial chunk may be a full chunk*/

    size_t first_partial_chunk_size = CHUNK_SIZE - first_partial_chunk_offset;
    
    //signals the function to exit early if we only have to read a part of the first partial chunk
    bool first_partial_chunk_early_exit_flag = false;

    //if we wont even be fully reading the first partial chunk
    if (amount <= first_partial_chunk_size)
    {
        first_partial_chunk_size = amount;
        first_partial_chunk_early_exit_flag = true;
    }

    StatusCode status = read_chunk_from_until(
        storage_man,
        file,
        first_partial_chunk_offset,
        first_partial_chunk_size,
        first_partial_chunk_index,
        &cur_buffer_addrs
    );

    if (status != SUCCESS)
    {
        return status;
    }

    if (first_partial_chunk_early_exit_flag)
    {
        return SUCCESS;
    }


    /* full_chunks_amount does not count the already processes first partial chunk even if it happens to be full
    amount is guaranteed by early exit to be greater than first_partial_chunk_size*/
    const size_t full_chunks_amount = (amount - first_partial_chunk_size) / CHUNK_SIZE;

    if (full_chunks_amount > 0)
    {
        status = read_full_chunks_and_update_buffer_pointer(
            file,
            first_partial_chunk_index + 1, //0th chunk has already been read
            storage_man,
            &cur_buffer_addrs,
            full_chunks_amount
        );

        if (status != SUCCESS) return status;
    }
    
    //this accounts for the amount of bytes less than CHUNK_SIZE that remain after we processes all the full chunks after the first partial chunk
    //no need to subtract full_chunks_amount * CHUNK_SIZE since it will be cancelled by % CHUNK_SIZE
    const size_t final_partial_chunk_size = (amount - first_partial_chunk_size) % CHUNK_SIZE;
    
    if (final_partial_chunk_size == 0)
    {
        //no final partial chunk to be processed
        return SUCCESS;
    }
    
    //we add 1 since full_chunks_amount does not account for the first partial chunk
    const size_t final_partial_chunk_index = first_partial_chunk_index + full_chunks_amount + 1;
    
    status = read_chunk_from_until(
        storage_man,
        file,
        0, //start at the begining of the chunk
        final_partial_chunk_size,
        final_partial_chunk_index,
        &cur_buffer_addrs
    );

    return status;
}

StatusCode read_full_chunks_and_update_buffer_pointer(
    File *file, 
    size_t relative_chunk_index,
    StorageMan *storage_man,
    char **buffer,
    size_t chunks_amount)
{
    //private preformance critical 
    //caller guarantees buffer can accommodate for chunks_amount * CHUNK_SIZE
    if (!file || !storage_man || !buffer)
    {
        return NULL_POINTER_PASSED;
    }
    
    StatusCode status;
    char *upper_lim = *buffer + (chunks_amount * CHUNK_SIZE);
    for (; *buffer < upper_lim; (*buffer) += CHUNK_SIZE)
    {
        size_t cur_chunk_index;
        status = get_chunk_index(file, relative_chunk_index++, &cur_chunk_index);
        
        if (status != SUCCESS)
        {
            return status;
        }

        status = file_read_chunk(
            file, 
            storage_man,
            cur_chunk_index, 
            *buffer,
            CHUNK_SIZE
        );

        if (status != SUCCESS)
        {
            return status;
        }
    }

    return SUCCESS;
}

StatusCode get_chunk_index(File *file, size_t relative_chunk_position, size_t *out_chunk_index)
{
    if (!file || !out_chunk_index)
    {
        return NULL_POINTER_PASSED;
    }

    if (relative_chunk_position >= file->allocated_size)
    {
        return INDEX_OUT_OF_BOUNDS;
    }

    size_t cur_chunk_extent_relative_start = 0;
    ChunkExtent *cur_chunk_extent =  file->data_chunk_extents;
    const ChunkExtent *upper_lim = cur_chunk_extent + MAX_FILE_CHUNK_EXTENTS_AMOUNT;
    for (;; cur_chunk_extent++)
    {
        if (cur_chunk_extent >= upper_lim)
        {
            return CHUNK_DOES_NOT_BELONG_TO_FILE;
        }

        if (cur_chunk_extent->is_empty) //chunk extents should be compact
        {
            return NOT_FOUND_CHUNKS_TERMINATED;
        }

        if (relative_chunk_position < cur_chunk_extent_relative_start + cur_chunk_extent->chunk_amount)
        {
            break;
        }

        cur_chunk_extent_relative_start += cur_chunk_extent->chunk_amount;
    }

    const size_t offset_within_chunk_extent = relative_chunk_position - cur_chunk_extent_relative_start;
    *out_chunk_index = cur_chunk_extent->start + offset_within_chunk_extent;

    return SUCCESS;
}

//copies data of size amount starting at start_offset from chunk into buffer. 
//modifies pointer to buffer to point after the end of the copied partial chunk
StatusCode read_chunk_from_until(
    StorageMan *storage_man,
    File *file, //necessary to enforce ownership
    size_t start_offset,
    size_t amount,
    size_t relative_chunk_index,
    char **buffer)
{
    //private helper
    //caller guarantees buffer can accommodate for partial chunk
    if (!storage_man || !file || !buffer)
    {
        return NULL_POINTER_PASSED;
    }

    if (start_offset + amount > CHUNK_SIZE)
    {
        return DATA_OVER_FLOW;
    }

    size_t chunk_index; 
    StatusCode status = get_chunk_index(file, relative_chunk_index, &chunk_index);
    
    if (status != SUCCESS)
    {
        return status;
    }

    char temp_buffer[CHUNK_SIZE];
    status = file_read_chunk(
        file, 
        storage_man, 
        chunk_index, 
        temp_buffer, 
        CHUNK_SIZE
    );
    
    if (status != SUCCESS)
    {
        return status;
    }

    size_t upper_lim = CHUNK_SIZE;

    //if we wont be fully finishing the chunk
    if (start_offset + amount < upper_lim)
    {
        upper_lim = start_offset + amount;
    }

    for (size_t i = start_offset; i < upper_lim; i++)
    {
        **buffer = temp_buffer[i];
        (*buffer)++;
    }
    
    return SUCCESS;
}

//The caller is Stateless. Thats why there isnt a writer abstraction
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