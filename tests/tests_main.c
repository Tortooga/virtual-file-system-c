#include "file_storage_tests.h"
#include <stdio.h>

typedef StatusCode (*TestFunc)(void);

StatusCode run_test(char *test_name, TestFunc test_func)
{
    if (!test_name || !test_func)
    {
        return NULL_POINTER_PASSED;
    }
    printf("%s Test returned with %d status code. \n", test_name, test_func());
    return SUCCESS;
}

int run_tests()
{
    run_test("File chunk extent allocator", test_file_allocate_chunks);
    run_test("File chunk extent deallocator", test_file_free_chunk_extent);
    return 0;
}

int main(void)
{
    return run_tests();
}

