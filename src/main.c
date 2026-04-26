#include "../include/storage.h"
#include "../include/settings.h"
#include "../include/files.h"
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
    ChunkExtent ext1;
    ChunkExtent ext2;

    file_init(&file, "hello.txt", 10);
    
    ext1.start = 0;
    ext1.chunk_amount = 2;
    ext1.is_empty = false;

    ext2.start = 5;
    ext2.chunk_amount = 2;
    ext2.is_empty = false;
    
    file.data_chunk_extents[0] = ext1;
    file.data_chunk_extents[1] = ext2;

    print_file(&file, true);

}