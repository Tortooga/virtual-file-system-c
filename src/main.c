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
    ChunkExtent *chunk_extent;
    file_allocate_chunks(&file, &storage_man, 2, &chunk_extent);
    file_write_chunk(&file, &storage_man, 0, "abcdefg", 8);

    char data[10];

    file_read_chunk(&file, &storage_man, 0, data, 10);

    printf("%s", data);
    return 0;
} 