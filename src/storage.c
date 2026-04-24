#include "../include/storage.h"
#include <stdio.h>
#include "../include/settings.h"

void mark_as_allocated(bool *offset, size_t amount);
void allocation_map_init(StorageMan *storage_man);
char *get_chunk_addr(StorageMan *storage_man, size_t chunk_pos);

int storage_man_init(
    StorageMan *storage_man,
    char *storage, 
    size_t storage_size,
    bool *allocation_map,
    size_t allocation_map_size)
    {
        if (!storage_man || !storage || !allocation_map)
        {
            return -1;
        }
        if (storage_size < CHUNK_SIZE * CHUNKS_AMOUNT)
        {
            return -2;
        }

        if (allocation_map_size < CHUNKS_AMOUNT)
        {
            return -3;
        }

        storage_man->allocation_map = allocation_map;
        storage_man->storage = storage;
        storage_man->storage_size = storage_size;
        
        allocation_map_init(storage_man);
        return 0;
    }

// Initialises every element of allocation_map to zero(false)
void allocation_map_init(StorageMan *storage_man)
{
    // Pointer validation on caller
    const bool *upper_lim  =  storage_man->allocation_map + CHUNKS_AMOUNT;
    for (bool *cur_addr = storage_man->allocation_map; cur_addr < upper_lim; cur_addr++)
    {
        *cur_addr = false;
    }
}

// Takes chunks amount and allocates space in virtual storage returning a pointer to the first chunk
int challoc(
    StorageMan *storage_man, 
    const size_t amount,
    size_t *out_first_chunk_index)
{
    if (!storage_man || !out_first_chunk_index)
    {
        return -1;
    }

    if (amount == 0 || amount > CHUNKS_AMOUNT)
    {
        return -2;
    }
    
    int cur_pos = 0;
    int available_continuous_chunks_count = 0;
    for (int i = 0; i < CHUNKS_AMOUNT; i++)
    {
        if (storage_man->allocation_map[i] == true)
        {
            cur_pos = i + 1; //Setting the cur position to the next object since this one is guarnteed to be 1
            available_continuous_chunks_count = 0;
            continue;
        }
        
        available_continuous_chunks_count++;

        if (available_continuous_chunks_count >= amount)
        {
            mark_as_allocated(storage_man->allocation_map + cur_pos, amount);
            *out_first_chunk_index = cur_pos;
            return 0;
        }
    }

    return -3;
}

int chfree(
    StorageMan *storage_man, 
    size_t chunk_pos)
{
    if (!storage_man || chunk_pos >= CHUNKS_AMOUNT)
    {
        return -1;
    }

    storage_man->allocation_map[chunk_pos] = false;
    return 0;    
}

int chwrite(
    StorageMan *storage_man, 
    size_t chunk_pos, 
    char *data, 
    size_t length)
{
    if (!storage_man)
    {
        return -1;
    }
    if (length > CHUNK_SIZE)
    {
        return -2;
    }
    if (chunk_pos >= CHUNKS_AMOUNT)
    {
        return -3;
    }
    //chunk should not be accessed if it isnt owned by something
    if (!storage_man->allocation_map[chunk_pos])
    {
        return -4;
    }

    char *chunk_addr = get_chunk_addr(storage_man, chunk_pos);

    //Validation guarantees length <= CHUNK_SIZE
    for (size_t i = 0; i < length; i++)
    {
        chunk_addr[i] = data[i];
    }

    return 0;
}

int chread(
    StorageMan *storage_man, 
    size_t chunk_index,
    char *out_data_array, 
    size_t data_array_length)
{
    //Edge cases
    if (!storage_man || !out_data_array)
    {
        return -1;
    }
    if (data_array_length < CHUNK_SIZE)
    {
        return -2;
    }
    if (chunk_index >= CHUNKS_AMOUNT)
    {
        return -3;
    }
    if (!storage_man->allocation_map[chunk_index])
    {
        return -4;
    }

    char *cur_addr = get_chunk_addr(storage_man, chunk_index);
    char *end_addr = cur_addr + CHUNK_SIZE;
    for (; cur_addr < end_addr; cur_addr++)
    {
        *out_data_array = *cur_addr;
        out_data_array++; 
    }

    return 0;
}

inline char *get_chunk_addr(StorageMan *storage_man, size_t chunk_pos)
{
    if (chunk_pos >= CHUNKS_AMOUNT || !storage_man)
    {
        return NULL;
    }
    return storage_man->storage + (chunk_pos * CHUNK_SIZE);
}

// Marks continuous amount of chunks as allocated
void mark_as_allocated(bool *offset, size_t amount)
{
    // Pointer validation on caller
    bool *upper_lim = offset + amount;
    for (; offset < upper_lim; offset++)
    {
        *offset = true;
    }
}

// prints storage continuously onto terminal
void print_storage(StorageMan *storage_man)
{
    for (int i = 0; i < storage_man->storage_size; i++)
    {
        if (i % CHUNK_SIZE == 0)
        {
            printf("\nC%d\n", i / CHUNK_SIZE);
        }
        printf(" %c |", storage_man->storage[i]);
    }

    printf("\n");
}

//prints allocation map continuously onto terminal
void print_allocation_map(StorageMan *storage_man)
{
    const bool *upper_lim = storage_man->allocation_map + CHUNKS_AMOUNT;
    for (bool *p = storage_man->allocation_map; p < upper_lim; p++)
    {
        printf(" %d |", *p);    
    }
    
    printf("\n");
}