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
    for (int i = 0; i < 8; i++)
    {
        file.data_chunk_extents[i].is_empty = false;
        file.data_chunk_extents[i].start = i;
        file.data_chunk_extents[i].chunk_amount = 1;
    }

    for (int i = 0; i < MAX_FILE_CHUNK_EXTENTS_AMOUNT; i++)
    {
        printf("%d,", !file.data_chunk_extents[i].is_empty);
    }

    printf("\n");
    StatusCode status = file_free_chunk_extent(&file, &file.data_chunk_extents[5], &storage_man);
    printf("%d\n", status);
    for (int i = 0; i < MAX_FILE_CHUNK_EXTENTS_AMOUNT; i++)
    {
        printf("%d,", !file.data_chunk_extents[i].is_empty);
    }

    return 0;
}