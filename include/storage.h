#ifndef STORAGE_H
#define STORAGE_H

#include <stdbool.h>
#include <stddef.h>
#include "settings.h"

typedef struct 
{
    char *storage; //The storage manager doesnt own the storage
    size_t storage_size;
    bool *allocation_map; //Todo: implement bit packing
} StorageMan;

typedef char CHUNK[CHUNK_SIZE];

int storage_man_init(
    StorageMan *storage_man,
    char *storage, 
    size_t storage_size,
    bool *allocation_map,
    size_t allocation_map_size);

char *challoc(StorageMan *storage_man, size_t amount);
void print_storage(StorageMan *storage_man);
void print_allocation_map(StorageMan *storage_man);

#endif