#ifndef FILE_LOGIC_H
#define FILE_LOGIC_H

#include "status.h"
#include "files.h"
#include "storage.h"


//Appends data to the end of a file
//Data is not treated as a C-String
StatusCode file_append(
    File *file, 
    StorageMan *storage_man,
    char* data,
    size_t data_length);

//Copies data starting at offset(bytes) into buffer
//Enforces buffer capacity   
StatusCode file_read_at(
    File *file,
    StorageMan *storage_man,
    size_t offset,
    size_t amount,
    char *buffer,
    size_t buffer_size
);

//frees all the files chunk extents
//updates file and storage manager meta data
StatusCode file_delete_data(File *file, StorageMan *storage_man);
#endif