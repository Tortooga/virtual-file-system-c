#include "file_storage_tests.h"
#include <stdio.h>

int run_tests()
{
    printf("Test Exited With Status Code %d\n", test_file_allocate_chunks());
    printf("Test Exited With Status Code %d\n", test_file_free_chunk_extent());
    return 0;
}

int main(void)
{
    return run_tests();
}

