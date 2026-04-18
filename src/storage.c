#include "../include/storage.h"
#include <stdio.h>

void print_storage(StorageSys storage_sys)
{
    for (char *p = storage_sys.storage; p < storage_sys.storage + storage_sys.storage_size; p++)
    {
        printf(" %c |", *p);
    }

    printf("\n");
}