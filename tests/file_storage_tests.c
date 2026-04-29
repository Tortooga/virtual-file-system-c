#include "../include/file_storage.h"
#include "test_settings.h"
#include <stdio.h>

StatusCode test_file_allocate_chunks()
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

    for (int i = 0; i < CHUNKS_AMOUNT - 5; i++)
    {
        storage_man.allocation_map[i] = true;
    }

    for (int i = CHUNKS_AMOUNT - 3; i < CHUNKS_AMOUNT; i++)
    {
        storage_man.allocation_map[i] = true;
    }

    ChunkExtent *chunk_extent;
    status = file_allocate_chunks(&file, &storage_man, 2, &chunk_extent);

    if (status != SUCCESS)
    {
        return status;
    }

    return SUCCESS;
}

StatusCode test_file_free_chunk_extent()
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

    ChunkExtent *chunk_extent;
    status = file_allocate_chunks(
        &file,
        &storage_man,
        5,
        &chunk_extent);
    
    if (status != SUCCESS)
    {
        return status;
    }

    status = file_free_chunk_extent(&file, chunk_extent, &storage_man);
    if (status != SUCCESS)
    {
        return status;
    }

    if (file.allocated_size > 0)
    {
        return TEST_ASSERTION_FAILED;
    }

    return SUCCESS;
}