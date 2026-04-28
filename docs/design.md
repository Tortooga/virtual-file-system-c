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
 
## CLI Client
CLI client translates CLI commands into file sys functionality
