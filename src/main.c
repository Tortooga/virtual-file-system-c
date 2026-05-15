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
    
    Folder root;
    Folder folder1;
    Folder folder2;
    Folder folder3;

    root_folder_init(&root);
    sub_folder_init(
        &folder1,
        "folder1",
        8,
        &root
    );

    sub_folder_init(
        &folder2,
        "folder2",
        8,
        &folder1
    );

    sub_folder_init(
        &folder3,
        "folder3",
        8,
        &folder2
    );

    sub_file_init(&file, &folder3);

    StatusCode get_parent_folders(VFSNode *node, Folder **out_parent_folders, size_t *out_parent_folders_amount);
    VFSNode node;
    node.type = FOLDER_NODE;
    node.node.folder = &root;

    Folder *folders[MAX_PATH_NODES_AMOUNT * (MAX_NAME_LENGTH + 1) + 1];
    size_t folder_amount;

    char path[MAX_PATH_NODES_AMOUNT * MAX_NAME_LENGTH];

} 