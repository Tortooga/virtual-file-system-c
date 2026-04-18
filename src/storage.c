#include "../include/storage.h"
#include <stdio.h>
#include "../include/settings.h"

//struct definition hidden in source file for encapsulation
typedef struct 
{
    char *storage;
    const int storage_size;
    const int chunk_size;
    bool *allocation_map; //Todo: implement bit packing
} StorageSys;


//prints storage continuously onto terminal
void print_storage(StorageSys storage_sys)
{
    for (char *p = storage_sys.storage; p < storage_sys.storage + storage_sys.storage_size; p++)
    {
        printf(" %c |", *p);
    }

    printf("\n");
}

//prints allocation map continuously onto terminal
void print_allocation_map(StorageSys storage_sys)
{
    for (char *p = storage_sys.allocation_map; p < storage_sys.allocation_map + CHUNK_SIZE; p++)
    {
        printf(" %d |", *p);    
    }
}