#include "../include/storage.h"
#include "../include/settings.h"
#include "../include/files.h"
#include "../include/file_storage.h"
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
    File file;
    file_init(&file, "hello.txt", 10);

    size_t first_chunk_index;
    int status = file_allocate_chunks(&file, &storage_man, 10, &first_chunk_index);

    printf("Status Code: %d\n", status);
    print_file(&file, true);
}