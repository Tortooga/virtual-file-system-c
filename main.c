#include "storage.h"
#include "settings.h"
#include "files.h"
#include "file_storage.h"
#include "file_logic.h"
#include "folders.h"
#include "queries.h"
#include "path_utils.h"
#include "entry_relocation.h"
#include "workspace.h"
#include "navigation.h"
#include "vfs_ops.h"
#include "file_io.h"


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

    File *file;

    status = vfs_sub_file_init(
        &file,
        &store,
        "file",
        5,
        folder
    );

    Workspace workspace;
    workspace_init(&store, &storage_man, &workspace);

    char message[] = "hello this is a message test whats up?";
    size_t message_length = strlen(message);

    status = ws_file_append(
        &workspace,
        "/folder/file",
        message,
        message_length
    );

    status = ws_file_append(
        &workspace,
        "/folder/file",
        message,
        message_length
    );

    print_file(file, true);
    printf("Append Status: %d\n", status);
    
    /*status = file_truncate_by(
        file,
        &storage_man,
        39
        );*/
        
        printf("Truncate Status: %d\n", status);
        
        char buffer[message_length * 5];
        
        status = file_read_at(
            file,
            &storage_man,
            0,
            file->allocated_size * CHUNK_SIZE,
            buffer,
            message_length * 5
        );
        
        
        printf("Read Status: %d\n", status);
        
        if (status != SUCCESS)
        {
            return status;
        }
        for (int i = 0; i < 85; i++)
    {
        printf("\n");
        
        status = file_delete_data(
            file,
            &storage_man
        );

        
        status = ws_file_append(
            &workspace,
            "/folder/file",
            message,
            message_length
        );
        
        status = ws_file_append(
            &workspace,
            "/folder/file",
            message,
            message_length
        );
        
        file_truncate_by(
            file,
            &storage_man,
            i
        );

        file_read_at(
            file,
            &storage_man,
            0,
            file->allocated_size * CHUNK_SIZE,
            buffer,
            message_length * 5
        );
        for (int i = 0; i < file->allocated_size * CHUNK_SIZE; i++)
        {
            printf("%c", buffer[i]);
        }
    }
    //print_storage(&storage_man);    
} 


void load_test_data(VFSEntryStore *store)
{
    #include <time.h>
    #include <stdio.h>

    clock_t start = clock();

    Folder *folder;
    File *file;

    /* =========================
    ROOT LEVEL
    ========================= */

    vfs_sub_folder_init(&folder, store, "home", 4, &store->root);
    Folder *f_home = folder;

    vfs_sub_folder_init(&folder, store, "etc", 3, &store->root);
    Folder *f_etc = folder;

    vfs_sub_folder_init(&folder, store, "var", 3, &store->root);
    Folder *f_var = folder;

    /* =========================
    HOME subtree
    ========================= */

    vfs_sub_folder_init(&folder, store, "user", 4, f_home);
    Folder *f_user = folder;

    vfs_sub_folder_init(&folder, store, "shared", 6, f_home);
    Folder *f_shared = folder;

    /* user subfolders */
    vfs_sub_folder_init(&folder, store, "documents", 9, f_user);
    Folder *f_documents = folder;

    vfs_sub_folder_init(&folder, store, "downloads", 9, f_user);
    Folder *f_downloads = folder;

    /* =========================
    ETC subtree
    ========================= */

    vfs_sub_folder_init(&folder, store, "nginx", 5, f_etc);
    Folder *f_nginx = folder;

    vfs_sub_folder_init(&folder, store, "conf", 4, f_nginx);
    Folder *f_conf = folder;

    /* =========================
    VAR subtree
    ========================= */

    vfs_sub_folder_init(&folder, store, "log", 3, f_var);
    Folder *f_log = folder;

    /* =========================
    FILES (19 total)
    ========================= */

    /* ---- home (2 files) ---- */
    vfs_sub_file_init(&file, store, "README", 6, f_home);
    vfs_sub_file_init(&file, store, "welcome.txt", 11, f_home);

    /* ---- user/documents (3 files) ---- */
    vfs_sub_file_init(&file, store, "resume.pdf", 10, f_documents);
    vfs_sub_file_init(&file, store, "notes.txt", 9, f_documents);
    vfs_sub_file_init(&file, store, "thesis.draft", 12, f_documents);

    /* ---- user/downloads (2 files) ---- */
    vfs_sub_file_init(&file, store, "setup.exe", 9, f_downloads);
    vfs_sub_file_init(&file, store, "movie.mkv", 9, f_downloads);

    /* ---- shared (2 files) ---- */
    vfs_sub_file_init(&file, store, "shared_info.txt", 15, f_shared);
    vfs_sub_file_init(&file, store, "rules.md", 8, f_shared);

    /* ---- etc (1 file) ---- */
    vfs_sub_file_init(&file, store, "hosts", 5, f_etc);

    /* ---- nginx/conf (4 files) ---- */
    vfs_sub_file_init(&file, store, "nginx.conf", 10, f_conf);
    vfs_sub_file_init(&file, store, "site1.conf", 10, f_conf);
    vfs_sub_file_init(&file, store, "default.conf", 12, f_conf);
    vfs_sub_file_init(&file, store, "mime.types", 10, f_conf);

    /* ---- var/log (3 files) ---- */
    vfs_sub_file_init(&file, store, "syslog", 6, f_log);
    vfs_sub_file_init(&file, store, "app.log", 7, f_log);
    vfs_sub_file_init(&file, store, "error.log", 9, f_log);

    clock_t end = clock();
    
    printf("Time: %f seconds\n",
        (double)(end - start) / CLOCKS_PER_SEC);
}