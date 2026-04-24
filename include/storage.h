#ifndef STORAGE_H
#define STORAGE_H

#include <stdbool.h>
#include <stddef.h>
#include "settings.h"

typedef struct 
{
    char *storage; //The storage manager doesnt own the storage
    size_t storage_size;
    bool *allocation_map; //Todo: implement bit packing
} StorageMan;


/*=============================*
 * Storage Operations          *
 *=============================*/



/* StorageMan Constructor. 
 * Does not own storage or allocation_map
 * Initialises allocation_map but does not intialise storage
 * 
 * Parameters:
 *   storage_man: StorageMan output param
 *   storage: ptr to first address in memory allocated for virtual storage
 *   storage_size: number of bytes allocated for virtual storage
 *   allocation_map: ptr to first address in memory allocated for chunk allocation map
 *   allocation_map_size: amount of memory allocated for allocation_map
 *  
 * Returns:
 *   0 if successful 
 *   -1 if a NULL pointer is passed to the function
 *   -2 if storage_size is less than the required amount 
 *   -3 if allocation_map_size is less than the required amount
 */
int storage_man_init(
    StorageMan *storage_man,
    char *storage, 
    size_t storage_size,
    bool *allocation_map,
    size_t allocation_map_size);


/* Storage chunks allocator.
 * Marks contiguous chunks in storage_man.allocation_map as allocated.
 * Does not initialise allocated storage .
 *
 * Parameters:
 *   storage_man: storage manager.
 *   amount: amount of chunks to be allocated
 *   out_first_chunk_index: output param for the index of the first chunk in storage.allocation_map
 * 
 * Returns:
 *   0 if successful
 *   -1 if a NULL pointer is passed to the function
 *   -2 if an invalid amount is passed(either 0 or greater than CHUNKS_AMOUNT)
 *   -3 if it failed to locate a series of contiguous chunks of size "amount" or more
*/
int challoc(
    StorageMan *storage_man, 
    size_t amount,
    size_t *out_first_chunk_index);

/* Marks chunk as free in storage_man.allocation_map.
 * Does not remove remove or overwrite data in storage_man.storage.
 * 
 * Parameters:
 *   storage_man: storage manager
 *   chunk_pos: index of chunk to be freed in storage_man.allocation_map
 * 
 * Returns:
 *   0 if successful
 *   -1 if a NULL pointer is passed to the function
 *   -2 if chunk_pos supplied is outside the bounds of CHUNKS_AMOUNT     
 */
int chfree(
    StorageMan *storage_man, 
    size_t chunk_pos);


/* Writes data into a chunk in storage_man.storage
 * Does initialise the rest of the chunk if the supplied data does not fill it
 *
 * Parameters:
 *   storage_man: storage manager
 *   chunk_pos: position of the chunk to be written into storage_man.allocation_map
 *   data: pointer to first address in the array where the data is to be copied from
 *   length: the length of the array to be copied from
 * 
 * Returns:
 *   0 if successful
 *   -1 if storage_man is a NULL pointer
 *   -2 if length is greater than CHUNK_SIZE
 *   -3 if chunk_pos is outside of the bounds of CHUNKS_AMOUNT
 *   -4 if the chunk to be written into is marked as free(not owned by anything)
 */
int chwrite(
    StorageMan *storage_man, 
    size_t chunk_pos, 
    char *data, 
    size_t length);


/* Reads data from a chunk in storage_man.storage by copying it into an output parameter array
 * Does not guarantee data is initialised
 * 
 * Parameters:
 *   storage_man: storage manager
 *   chunk_index: index of the chunk to be read from
 *   out_data_array: output parameter for the array the chunk data is to be copied into
 *   array_length: length of the array the data is going to be copied into(only used for validation)
 * 
 * Returns:
 *   -1 if NULL pointer is passed into the function
 *   -2 if array_length is less than CHUNK_SIZE(will not fit the data)
 *   -3 if chunk_index is outside the bounds of CHUNKS_AMOUNT
 *   -4 if the chuck to be read is marked as free in storage_man.allocation_map
 */ 
int chread(
    StorageMan *storage_man, 
    size_t chunk_index,
    char *out_data_array, 
    size_t array_length);

/*========================*
 * Debugging helpers      *
 *========================*/

//Formats and prints storage_man.storage contiguously onto the terminal.
void print_storage(StorageMan *storage_man);

//Formats and prints storage_man.allocation_map contiguously onto the terminal.
void print_allocation_map(StorageMan *storage_man);

#endif