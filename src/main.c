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

    storage_man.allocation_map[0] = 1;
    storage_man.allocation_map[CHUNKS_AMOUNT - 1] = 1;
    print_allocation_map(&storage_man);

    chwrite(&storage_man, CHUNKS_AMOUNT - 1, "Hello", 6);
    print_storage(&storage_man);
}