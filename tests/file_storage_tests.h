#ifndef FILE_STORAGE_TESTS_H
#define FILE_STORAGE_TESTS_H

#include "test_settings.h"
#include "../include/status.h"

StatusCode test_file_allocate_chunks();
StatusCode test_file_free_chunk_extent();

#endif