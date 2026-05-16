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

StatusCode sub_entry_name_is_unique(Folder *parent_folder, char *name, size_t name_length, bool *out_result);
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

    StatusCode vfs_sub_file_init(
        VFSEntryStore *entry_store,
        char *file_name,
        size_t file_name_length,
        Folder *parent_folder
    );

    StatusCode status = vfs_sub_file_init(
        &store,
        "config.sys",
        11,
        &store.folders[0]
    );

    printf("status: %d\n", status);

    File *file;

    search_sub_file(&store.folders[0], "config.sys", 11, &file);
    printf("%s", file->name);
} 

void load_test_data(VFSEntryStore *store)
{
    #include <time.h>
    #include <stdio.h>

    clock_t start = clock();

    file_init(&store->files[0],  "hello.txt", 10);
    file_init(&store->files[1],  "data.bin", 9);
    file_init(&store->files[2],  "config.sys", 11);
    file_init(&store->files[3],  "readme.md", 10);
    file_init(&store->files[4],  "image.png", 10);
    file_init(&store->files[5],  "hello.txt", 10);
    file_init(&store->files[6],  "archive.zip", 12);
    file_init(&store->files[7],  "data.bin", 9);
    file_init(&store->files[8],  "notes.md", 9);
    file_init(&store->files[9],  "config.sys", 11);
    file_init(&store->files[10], "log.txt", 8);
    file_init(&store->files[11], "backup.tar", 11);
    file_init(&store->files[12], "image.png", 10);
    file_init(&store->files[13], "todo.txt", 9);
    file_init(&store->files[14], "hello.txt", 10);
    file_init(&store->files[15], "data.bin", 9);
    file_init(&store->files[16], "report.pdf", 11);
    file_init(&store->files[17], "notes.md", 9);
    
    
    root_folder_init(&store->root);
    
    sub_folder_init(&store->folders[0], "folder1", 8, &store->root);
    sub_folder_init(&store->folders[1], "folder2", 8, &store->folders[0]);
    sub_folder_init(&store->folders[2], "folder3", 8, &store->folders[0]);
    sub_folder_init(&store->folders[3], "folder4", 8, &store->root);
    sub_folder_init(&store->folders[4], "folder5", 8, &store->folders[3]);
    sub_folder_init(&store->folders[5], "folder6", 8, &store->folders[3]);
    sub_folder_init(&store->folders[6], "folder7", 8, &store->folders[2]);
    sub_folder_init(&store->folders[7], "folder8", 8, &store->root);
    sub_folder_init(&store->folders[8], "folder9", 8, &store->folders[7]);
    sub_folder_init(&store->folders[9], "folder10", 9, &store->folders[7]);
    
    sub_file_init(&store->files[0],  &store->folders[0]);
    sub_file_init(&store->files[1],  &store->folders[1]);
    sub_file_init(&store->files[2],  &store->folders[2]);
    sub_file_init(&store->files[3],  &store->folders[0]);
    sub_file_init(&store->files[4],  &store->folders[3]);
    sub_file_init(&store->files[5],  &store->folders[2]);
    sub_file_init(&store->files[6],  &store->folders[4]);
    sub_file_init(&store->files[7],  &store->folders[1]);
    sub_file_init(&store->files[8],  &store->folders[5]);
    sub_file_init(&store->files[9],  &store->folders[3]);
    sub_file_init(&store->files[10], &store->folders[6]);
    sub_file_init(&store->files[11], &store->folders[7]);
    sub_file_init(&store->files[12], &store->folders[4]);
    sub_file_init(&store->files[13], &store->folders[8]);
    sub_file_init(&store->files[14], &store->folders[6]); //
    sub_file_init(&store->files[15], &store->folders[1]);
    sub_file_init(&store->files[16], &store->folders[9]);
    sub_file_init(&store->files[17], &store->folders[5]);
    sub_file_init(&store->files[18], &store->folders[7]);


    for (int i = 0; i < 19; i++)
    {
        store->files_allocation_map[i] = true;
    }

    for (int i = 0; i < 10; i++)
    {
        store->folders_allocation_map[i] = true;
    }
    
    clock_t end = clock();
    
    printf("Time: %f seconds\n",
        (double)(end - start) / CLOCKS_PER_SEC);
}