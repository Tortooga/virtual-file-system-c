#include "../include/storage.h"
#include "../include/settings.h"
#include "../include/files.h"
#include "../include/file_storage.h"
#include "../include/file_logic.h"
#include "../include/folders.h"

#include <stdio.h>

bool name_eq(char *initialised_name, char *uninitialised_name, size_t uninitialised_name_length);
StatusCode sub_entry_name_is_unique(Folder *parent_folder, char *name, size_t name_length, bool *out_result);

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
    
    Folder root;

    StatusCode status = root_folder_init(&root);

    printf("%d", status);
    Folder folder;

    status = sub_folder_init(
        &folder,
        "test",
        12,
        &root
    );

    printf("%d\n", status);
    printf("%s\n", folder.name);
    printf("%s\n", (folder.parent_folder)->name);
     
    return 0;
} 