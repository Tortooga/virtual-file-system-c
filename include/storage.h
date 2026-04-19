#include <stdbool.h>
#include <stddef.h>

typedef struct 
{
    char *storage; //The storage manager doesnt own the storage
    size_t storage_size;
    bool *allocation_map; //Todo: implement bit packing
} StorageMan;


int storage_man_init(
    StorageMan *storage_man,
    char *storage, 
    size_t storage_size,
    bool *allocation_map,
    size_t allocation_map_size);

void print_storage(StorageMan storage_man);
