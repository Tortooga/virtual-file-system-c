#include "../include/storage.h"
#include "../include/settings.h"
#include "../include/files.h"
#include "../include/file_storage.h"
#include "../include/file_logic.h"
#include "../include/folders.h"
#include "../include/queries.h"
#include "../include/path_parser.h"
#include "../include/vfs_context.h"

#include <stdio.h>
#include <string.h>

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
    File file2;
    file_init(&file, "hello.txt", 11);
    file_init(&file2, "hello2.txt", 12);
    
    Folder root;

    StatusCode status = root_folder_init(&root);

    Folder folder1;
    Folder folder2;

    status = sub_folder_init(
        &folder1,
        "test",
        5,
        &root
    );
    status = sub_folder_init(
        &folder2,
        "test2",
        5,
        &root
    );
    status = sub_file_init(
        &file2,
        &folder1
    );
    status = sub_file_init(
        &file,
        &folder1
    );
    
    for (size_t i = 0; i < MAX_SUB_FILES_AMOUNT; i++)
    {
        if (!folder1.sub_files[i])
        {
            continue;
        }
        printf("%s\n", folder1.sub_files[i]->name);
    }

    printf("after unlinking");

    unlink_sub_file(&folder1, &file);
    unlink_sub_file(&folder1, &file2);

    for (size_t i = 0; i < MAX_SUB_FILES_AMOUNT; i++)
    {
        if (!folder1.sub_files[i])
        {
            continue;
        }
        printf("%s\n", folder1.sub_files[i]->name);
    }

    return 0;
} 