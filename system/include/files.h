#ifndef FILES_H
#define FILES_H

#include "storage.h"

//if is_empy then start and chunk_amount are unitialised
typedef struct
{
    bool is_empty;
    size_t start;
    size_t chunk_amount;
} ChunkExtent;

//Forward decloration of Folder so that a files can store a pointer to its parent
//Folders do not own or control life time of file. Both are owned by VFSEntryStore
typedef struct Folder Folder;


//At this layer extension is a part of the name
typedef struct
{
    size_t allocated_size; //tracked at allocation and deallocation
    Folder *parent_folder;

    ChunkExtent data_chunk_extents[MAX_FILE_CHUNK_EXTENTS_AMOUNT];
    char name[MAX_NAME_LENGTH]; //null terminated
} File;

//Initialises file
//validates name
//marks each file chunk extent to be empty (file.chunk_extents.is_empty = true) 
//sets parent file to NULL(this layer is not in charge of linking entries)
StatusCode file_init(
    File *out_file,
    char *file_name,
    size_t file_name_length 
);

//prints metadata about file onto terminal
void print_file(
    File *file, 
    bool include_extents);

#endif