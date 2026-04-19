#include "../include/storage.h"
#include "../include/settings.h"

int main(int argc, char *argv)
{
    StorageMan storage_man;
    char storage[CHUNKS_AMOUNT * CHUNK_SIZE];
    bool allocation_map[CHUNKS_AMOUNT];

    for (int i = 0; i < CHUNK_SIZE * CHUNKS_AMOUNT; i++)
    {
        storage[i] = 90;
    }

    storage_man_init
    (
        &storage_man,
        storage,
        CHUNKS_AMOUNT * CHUNK_SIZE,
        allocation_map,
        CHUNKS_AMOUNT
    );

    print_storage(storage_man);
}