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
    file_init(&file, "hello.txt", 11);
    
    char data[] = "hello this is test data";


    file_append(&file, &storage_man, data, strlen(data));
    file_append(&file, &storage_man, data, strlen(data));
    file_append(&file, &storage_man, data, strlen(data));
    
    print_file(&file, true);    

    StatusCode chunk_extent_right_shift(ChunkExtent *position, size_t shift_amount, File *file);


    StatusCode status = chunk_extent_right_shift(
        &file.data_chunk_extents[0], 5, &file 
    );

    printf("%d\n", status);

    
    print_file(&file, true);
    return 0;
} 