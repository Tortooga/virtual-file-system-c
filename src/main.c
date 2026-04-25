#include "../include/storage.h"
#include "../include/settings.h"
#include <stdio.h>

int main(int argc, char *argv)
{
    StorageMan storage_man;
    char storage[CHUNKS_AMOUNT * CHUNK_SIZE];
    bool allocation_map[CHUNKS_AMOUNT];

    for (int i = 0; i < CHUNK_SIZE * CHUNKS_AMOUNT; i++)
    {
        storage[i] = 0;
    }

    storage_man_init
    (
        &storage_man,
        storage,
        CHUNKS_AMOUNT * CHUNK_SIZE,
        allocation_map,
        CHUNKS_AMOUNT
    );

    size_t first_chunk_index;
    
    challoc(&storage_man, 2, &first_chunk_index);
   
    print_allocation_map(&storage_man);

    chwrite(&storage_man, first_chunk_index, "hello", 6);
    print_storage(&storage_man);

    char msg[CHUNK_SIZE];

    int result = chread(&storage_man, first_chunk_index, msg, CHUNK_SIZE);

    printf("%s\n", msg);

}