#include <stdbool.h>
#define STORAGE_SIZE 5

typedef struct 
{
    bool *allocation_map; //Todo: implement bit packing
    char *storage;
    const int chunk_size;
} StorageSys;

void print_storage(StorageSys storage_sys);