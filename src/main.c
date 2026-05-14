#include "../include/storage.h"
#include "../include/settings.h"
#include "../include/files.h"
#include "../include/file_storage.h"
#include "../include/file_logic.h"
#include "../include/folders.h"
#include "../include/queries.h"
#include "../include/path_utils.h"
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
    file_init(&file, "hello.txt", 11);
    
    StatusCode append_node_name_to_path(const char *node_name, char *path_end, const size_t available_bytes_amount);
    const char name[] = "test";
    char path[25] = "file1/file2";
    StatusCode status = append_node_name_to_path(name, &path[11], 13);
    printf("%d\n", status);
    printf("%s", path);
    return 0;
} 