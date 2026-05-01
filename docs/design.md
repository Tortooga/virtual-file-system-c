# Virtual file System

## Storage
Virtual disk stored in fixed sized array(contiguous flat storage)

## Storage System
The Storage system logically chunks the storage and keeps track of the status of each chunk(whether its occupied or free)

It handles all storage functionally including allocation, deallocation and writing.

## Storage Chunks

A chunk is a position in the allocaiton map. Its only representation is its index in the allocation map. It functions as the smallest unit of storage addressable by the allocater and de allocator. It is also used for data retreival as its index can be transformed into real storage.

## Chunk Size Trade-off
If chunk size is too high, storage wastefully fragments and big portions are left unaccessible.
If chunk size is too low, file meta data grows as each file will have to track alot of storage chunks.

## File Chunks
A chunk owned by a file is assumed to be occupied with data when it comes to the allocation and writing layer. While reading, however, if the reader encounters a null terminator it skips to the next chunks.

## File System
File system address's storage in logical chunks.

File system is unable to directly access storage addresses. It can only interract with storage through storage manager. It calles storage system functions and refrences desired positions using chunk indices which the storage manager later maps into storage addresses(maping is extremely inexpensive). 

File system manager administers IO, allocation/deallocation and functionality.

It stores meta data about files and folders.

Files are mapped to storage locations using extent-based allocation

## Compact Extents Sequencing
The order of elements in file.data_chunks_extents indicates the order of how the data is to be read.
compactness is enforced by shifting to the right when a chunk extent is freed. This facilitates append mode

## Copy on Write
when a file is edited on modification mode the data is copied into memory, modified in memory and then rewritten into virtual storage

## File Chunk Allocator And Deallocator
file_allocate_chunks and file_free_chunks are abstractions over the storage alloc and dealloc functions challoc and chfree. they call these functions updating a files meta data accordingly 

## File Storage layer
this is the intermediate layer between raw storage IO and file IO. File cannot access storage other than through it. It validates and registers relevent file meta data each time a request is made. It verifies ownership before allowing a file to read or write from a specified chunk.

## File Logic layer
Abstracts the file storage layer by exposing functions that enable the caller to preform IO tasks on files without having to explicitly address and reason about file storage chunks

## RollBack-Based Write Partial Failure Handeling
Since our system relies on and enforces file ownership, such that a chunk can not be accessed by anything other than the file that owns it, failure must be handeled by rolling back, deallocating chunks and modifying file meta-data. This makes for a simpler system but more explicit failure handelling is required as opposed to commit-based partial failure handelling, where chunks are not added into file meta data untill the write processes succeeds.

## File 
## CLI Client
CLI client translates CLI commands into file sys functionality
