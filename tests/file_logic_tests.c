#include "../include/file_logic.h"
#include "../include/files.h"
#include <stdio.h>
#include <string.h>


StatusCode test_file_append()
{
    StatusCode status;

    File file;
    status = file_init(&file, "TestFile.txt", 13);

    if (status != SUCCESS)
    {
        return status;
    }

    char storage[CHUNKS_AMOUNT * CHUNK_SIZE];
    bool allocation_map[CHUNKS_AMOUNT];
    StorageMan storage_man;
    status = storage_man_init(
        &storage_man,
        storage,
        CHUNK_SIZE * CHUNKS_AMOUNT,
        allocation_map,
        CHUNKS_AMOUNT
    );

    if (status != SUCCESS)
    {
        return status;
    }

    char data[] = "This is test data.";
    size_t data_length = sizeof(data) / sizeof(data[0]);

    file_append(
        &file,
        &storage_man,
        data,
        data_length
    );

    char buffer[data_length];
    
    file_read_at(
        &file,
        &storage_man,
        0,
        data_length,
        buffer,
        data_length
    );

    if (strncmp(data, buffer, data_length) != 0)
    {
        return TEST_ASSERTION_FAILED;
    }
    return SUCCESS;
}