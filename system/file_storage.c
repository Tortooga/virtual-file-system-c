#include "file_storage.h"
/*
This layer enforces file ownership on storage operations
*/
int get_available_chunk_extent_index(File *file);
StatusCode free_chunk_extent(ChunkExtent *chunk_extent, StorageMan *storage_man);
void chunk_extent_copy_range(ChunkExtent *target_chunk_extent, ChunkExtent *copy_chunk_extent);
StatusCode chunk_extent_left_shift(ChunkExtent *empty_position, File *file);
bool file_owns_chunk(File *file, size_t chunk_pos);
StatusCode get_last_chunk_extent(File *file, ChunkExtent **out_chunk_extent);

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

StatusCode file_write_chunk(
    File *file, 
    StorageMan *storage_man, 
    size_t chunk_pos,
    char *data,
    size_t data_length)
{
    if (!file || !storage_man || !data)
    {
        return NULL_POINTER_PASSED;
    }

    if (chunk_pos >= CHUNKS_AMOUNT)
    {
        return INVALID_ARGUMENT;
    }

    //This validation is the main purpose of this function
    if (!file_owns_chunk(file, chunk_pos))
    {
        return CHUNK_DOES_NOT_BELONG_TO_FILE;
    }

    //data_length validated by chwrite
    //valid data is guaranteed by caller
    StatusCode status = chwrite(
        storage_man,
        chunk_pos,
        data,
        data_length
    );

    return status;
}

StatusCode file_read_chunk(
    File *file, 
    StorageMan *storage_man,
    size_t chunk_pos,
    char *out_data,
    size_t data_length)
{
    if (!file || !storage_man || !out_data)
    {
        return NULL_POINTER_PASSED;
    }

    if (chunk_pos >= CHUNKS_AMOUNT)
    {
        return INVALID_ARGUMENT;
    }

    if (!file_owns_chunk(file, chunk_pos))
    {
        return CHUNK_DOES_NOT_BELONG_TO_FILE;
    }

    //data_length validated by chwrite
    StatusCode status = chread(
        storage_man,
        chunk_pos,
        out_data,
        data_length
    );

    return status;
}

bool file_owns_chunk(File *file, size_t chunk_pos)
{
    //private helper only called from file_write_chunk does not validate file or chunk_pos 
    for (int i = 0; i < MAX_FILE_CHUNK_EXTENTS_AMOUNT; i++)
    {
        //data chunks array guaranteed to be compact. empty chunk implies we hit the end of occupied chunks.
        if (file->data_chunk_extents[i].is_empty)
        {
            return false;
        }
        //if the chunk belongs to this extent return true
        if (
            file->data_chunk_extents[i].start <= chunk_pos
            && file->data_chunk_extents[i].start + file->data_chunk_extents[i].chunk_amount > chunk_pos)
        {
            return true;
        }
    }

    return false;
}

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


//If you ever want to implement "write at"

//Assumes file.data_chunk_extents is compact.
//Shifts all chunks beyond and including the specified position to the right by shift_amount
//Positions shifted from must immediately be filled otherwise compactness will be broken
StatusCode chunk_extent_right_shift(ChunkExtent *position, size_t shift_amount, File *file)
{
    if (!position || !file)
    {
        return NULL_POINTER_PASSED;
    }

    if (shift_amount == 0)
    {
        return INVALID_OPERATION;
    }

    if (position->is_empty)
    {
        return INVALID_ARGUMENT;
    }

    //Exclusive upper lim for chunk extent array
    const ChunkExtent *chunk_extents_upper_lim = &file->data_chunk_extents[0] + MAX_FILE_CHUNK_EXTENTS_AMOUNT;
    //Inclusive lower lim
    const ChunkExtent *chunk_extent_lower_lim = &file->data_chunk_extents[0];
    if (position < chunk_extent_lower_lim || position >= chunk_extents_upper_lim)
    {
        return INDEX_OUT_OF_BOUNDS;
    }

    ChunkExtent *last_addrs;
    StatusCode status = get_last_chunk_extent(file, &last_addrs);

    if (status != SUCCESS)
    {
        return status;
    }

    if (last_addrs + shift_amount >= chunk_extents_upper_lim)
    {
        return DATA_OVER_FLOW;
    }

    //With the compactness invariant and since we made sure position is not an empty chunk during validation
    //if cur_addrs starts last_addrs and keeps going down it is guaranteed to encounter position
    for (ChunkExtent *cur_addrs = last_addrs; cur_addrs >= position; cur_addrs--)
    {
        //This should never trigger
        if (cur_addrs < chunk_extent_lower_lim)
        {
            return INDEX_OUT_OF_BOUNDS; //FATAL
        }

        //guaranteed to be within bounds
        chunk_extent_copy_range(cur_addrs + shift_amount, cur_addrs);
        cur_addrs->is_empty = true;
        cur_addrs[shift_amount].is_empty = false;
    }

    return SUCCESS;
}

//outputs the address of the last position in a files sequence of chunk extents
//assumes chunk extents are compact 
//returns error if the occupied last chunk extent is the last chunk extent in the array 
StatusCode get_last_chunk_extent(File *file, ChunkExtent **out_chunk_extent)
{
    if (!file || !out_chunk_extent)
    {
        return NULL_POINTER_PASSED;
    }
    
    *out_chunk_extent = NULL;

    if (file->data_chunk_extents[0].is_empty)
    {
        return DATA_SET_IS_EMPTY;
    }

    for (size_t i = 1; i < MAX_FILE_CHUNK_EXTENTS_AMOUNT; i++)
    {
        if (file->data_chunk_extents[i].is_empty)
        {
            *out_chunk_extent = &file->data_chunk_extents[i - 1];
            return SUCCESS;
        }
    }

    //Since the caller will be attempting to shift data to the right
    return DATA_OVER_FLOW;
}

//truncates a chunk extent
StatusCode file_truncate_chunk_extent(File *file, ChunkExtent *chunk_extent, const size_t amount, StorageMan *storage_man)
{
    if (!chunk_extent || !file || !storage_man)
    {
        return NULL_POINTER_PASSED;
    }

    if (chunk_extent->is_empty)
    {
        return INVALID_ARGUMENT;
    }

    if (amount > chunk_extent->chunk_amount)
    {
        return INDEX_OUT_OF_BOUNDS;
    }

    //We entirely remove the chunk extents meta data if it is going to become empty
    if (amount == chunk_extent->chunk_amount)
    {
        return file_free_chunk_extent(file, chunk_extent, storage_man);
    }

    StatusCode status;
    
    size_t extent_upper_lim_index = chunk_extent->start + chunk_extent->chunk_amount;
    
    for (
        size_t cur_chunk = extent_upper_lim_index - amount; 
        cur_chunk < extent_upper_lim_index; 
        cur_chunk++
    )
    {
        status = chfree(storage_man, cur_chunk);

        if (status != SUCCESS)
        {
            return status;
        }
        
        //mutating meta data at each iteration 
        // so partial failure leads to partial deallocation with accurate file and chunk extent meta data
        file->allocated_size -= 1;
        chunk_extent->chunk_amount -= 1;
    }

    return SUCCESS;
}


void chunk_extent_copy_range(ChunkExtent *target_chunk_extent, ChunkExtent *copy_chunk_extent)
{
    //Both target and copy are guaranteed to have is_empty = false
    target_chunk_extent->chunk_amount = copy_chunk_extent->chunk_amount;
    target_chunk_extent->start = copy_chunk_extent->start;
}


//Partial failure causes partial deallocation. TODO: implement early system exit
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