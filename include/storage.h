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


/*=============================*
 * Storage Operations          *
 *=============================*/


int storage_man_init(
    StorageMan *storage_man,
    char *storage, 
    size_t storage_size,
    bool *allocation_map,
    size_t allocation_map_size);
int challoc(
    StorageMan *storage_man, 
    size_t amount,
    size_t *out_first_chunk_index);

int chfree(
    StorageMan *storage_man, 
    size_t chunk_pos);
int chwrite(
    StorageMan *storage_man, 
    size_t chunk_pos, 
    char *data, 
    int length);
int chread(
    StorageMan *storage_man, 
    size_t chunk_index,
    char *out_data_array, 
    size_t array_length);

/*========================*
 * Debugging helpers      *
 *========================*/

void print_storage(StorageMan *storage_man);
void print_allocation_map(StorageMan *storage_man);

#endif