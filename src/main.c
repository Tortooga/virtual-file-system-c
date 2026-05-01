#include "../include/storage.h"
#include "../include/settings.h"
#include "../include/files.h"
#include "../include/file_storage.h"
#include "../include/file_logic.h"

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

    StatusCode status = file_append(
        &file,
        &storage_man,
        "first data test please stand buy.",
        34
    );
    
    printf("%d", status);
    print_storage(&storage_man);
    print_file(&file, true);
    printf("%zu", storage_man.free_chunk_count);
    return 0;
} 