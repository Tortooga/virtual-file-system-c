#include "storage.h"

//if is_empy then start and chunk_amount are unitialised
typedef struct
{
    bool is_empty;
    size_t start;
    size_t chunk_amount;
} ChunkExtent;

//At this layer extension is a part of the name
typedef struct
{
    ChunkExtent data_chunk_extents[MAX_FILE_CHUNK_EXTENTS_AMOUNT];
    char name[MAX_FILE_NAME_LENGTH];
} File;


//caller is responsible for intialising name and extension
int file_init(
    File *out_file,
    char *file_name,
    size_t file_name_length
);