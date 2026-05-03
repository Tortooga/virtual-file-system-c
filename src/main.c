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
    
    char msg[] = "This is the first test lol ol lol.";
    size_t msg_size = sizeof(msg) / sizeof(msg[0]);

    char buffer[msg_size];
    StatusCode status = file_append(
        &file,
        &storage_man,
        msg,
        msg_size
    );
    

    status = file_read_at(
        &file,
        &storage_man,
        0, msg_size, buffer,
        msg_size 
    );

    for (int i = 0; i < msg_size; i++)
    {
        printf("%c",buffer[i]);
    }
    printf("\n");
    print_file(&file, true);
    

    print_storage(&storage_man);
    return 0;
} 