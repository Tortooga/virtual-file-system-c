#include "../include/filesys.h"
#include <string.h>
#include <stdio.h>

int init_empty_extensions(File *file);

int file_init(
    File *out_file,
    char *file_name,
    size_t file_name_length)
    {
        if (!out_file || !file_name)
        {
            return -1;
        }
        if (file_name_length > MAX_FILE_NAME_LENGTH)
        {
            return -2;
        }
        //Caller guarantees file_name is initialised
        memcpy(out_file->name, file_name, file_name_length);

        init_empty_extensions(out_file);
        return 0;
    }

int init_empty_extensions(File *file)
{
    if (!file)
    {
        return -1;
    }
    ChunkExtent *cur_extent_addrs = file->data_chunk_extents;
    const ChunkExtent *extent_addrs_upper_lim = cur_extent_addrs + MAX_FILE_CHUNK_EXTENTS_AMOUNT;

    for(; cur_extent_addrs < extent_addrs_upper_lim; cur_extent_addrs++)
    {
        cur_extent_addrs->is_empty = true;
    }
    return 0;
}
