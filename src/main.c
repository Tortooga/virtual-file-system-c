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
    
    Folder folder1;

    for (int i = 0; i < MAX_SUB_FOLDERS_AMOUNT; i++)
    {
        folder1.sub_folders[i] = NULL;
    }
    for (int i = 0; i < MAX_SUB_FILES_AMOUNT; i++)
    {
        folder1.sub_files[i] = NULL;
    }
    Folder folder2;
    
    StatusCode status = sub_folder_init(
        &folder1,
        "test_folder",
        12,
        &folder1
    );

    status = sub_folder_init(
        &folder2,
        "test_polder",
        12,
        &folder1
    );

    printf("%d\n", status);
    //printf("%s\n", folder2.name);
    //printf("%s\n", (folder2.parent_folder)->name);
     
    bool is_available;
    sub_entry_name_is_unique(&folder1, "test_folder", 12, &is_available);
    printf("are equal: %d", is_available);
    return 0;
} 