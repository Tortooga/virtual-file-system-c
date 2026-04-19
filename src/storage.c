#include "../include/storage.h"
#include <stdio.h>
#include "../include/settings.h"

void mark_as_allocated(bool *offset, size_t amount);
void allocation_map_init(StorageMan *storage_man);

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
            return -1; //Todo add error logging
        }

        if (allocation_map_size < CHUNKS_AMOUNT)
        {
            return -1;
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
char *challoc(StorageMan *storage_man, const size_t amount)
{
    if (!storage_man || amount == 0)
    {
        return NULL;
    }
    
    int cur_pos = 0;
    int available_continuous_chunks_count = 0;
    for (int i = 0; i < CHUNKS_AMOUNT; i++)
    {
        
        printf("%d: %d\n", i, available_continuous_chunks_count);
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
            return storage_man->storage + (cur_pos * CHUNK_SIZE);
        }
    }

    return NULL;
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
    const char *upper_lim = storage_man->storage + storage_man->storage_size;
    for (char *p = storage_man->storage; p < upper_lim; p++)
    {
        printf(" %c |", *p);
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