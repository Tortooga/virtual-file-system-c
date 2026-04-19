#include "../include/storage.h"
#include <stdio.h>
#include "../include/settings.h"

int storage_man_init(  //FIX DESIGN
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
        
        return 0;
    }

//prints storage continuously onto terminal
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
    const bool *upper_lim = storage_man->allocation_map + CHUNK_SIZE;
    for (bool *p = storage_man->allocation_map; p < upper_lim; p++)
    {
        printf(" %d |", *p);    
    }
}