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

/*
 DISCLAIMER: This is only a testing play ground  
*/

void load_test_data(VFSEntryStore *store);

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
    
    VFSEntryStore store;
    vfs_entry_store_init(&store);

    load_test_data(&store);

    VFSNode nodes[10];
    size_t nodes_amount;

    StatusCode status = global_search_nodes_by_name(
        "setup.exe",
        10,
        &store,
        nodes,
        10,
        &nodes_amount
    );

    Folder *folder;

    status = vfs_sub_folder_init(&folder, &store, "folder", 7, &store.root);

    printf("Folder creation status: %d\n", status);

    File *file;

    status = vfs_sub_file_init(
        &file,
        &store,
        "file",
        5,
        folder
    );

    printf("File creation status: %d\n", status);

    //print_entry_store(&store);
} 

void load_test_data(VFSEntryStore *store)
{
    #include <time.h>
    #include <stdio.h>

    clock_t start = clock();



    clock_t end = clock();
    
    printf("Time: %f seconds\n",
        (double)(end - start) / CLOCKS_PER_SEC);
}