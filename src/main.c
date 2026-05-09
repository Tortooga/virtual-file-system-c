#include "../include/storage.h"
#include "../include/settings.h"
#include "../include/files.h"
#include "../include/file_storage.h"
#include "../include/file_logic.h"
#include "../include/folders.h"
#include "../include/queries.h"
#include "../include/path_parser.h"
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
    file_init(&file, "hello.txt", 10);
    
    Folder root;

    StatusCode status = root_folder_init(&root);

    Folder folder1;

    status = sub_folder_init(
        &folder1,
        "test",
        5,
        &root
    );

    folder1.sub_files[4] = &file;

    char path[] = "test/hello.txt";
    size_t path_length = strlen(path);
    char nodes[10][MAX_NAME_LENGTH];
    size_t nodes_amount;
    VFSNode node;
    
    status = resolve_path(
        path,
        path_length,
        &root,
        &node
    );

    printf("Returned with status code %d\n", status);
    if (status != SUCCESS)
    {
        return -1;
    }
    printf("Type: %d\n", node.type);
    printf("are equal: %d\n", node.node.file == &file);
    printf("%s\n", node.node.file->name);
    return 0;
} 