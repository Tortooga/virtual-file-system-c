#include <stdbool.h>

typedef struct 
{
    char *storage;
    const int storage_size;
    const int chunk_size;
    bool *allocation_map; //Todo: implement bit packing
} StorageSys;

void print_storage(StorageSys storage_sys);