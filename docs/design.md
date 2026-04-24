# Virtual file System

## Storage
Virtual disk stored in fixed sized array(contiguous flat storage)

## Storage System
The Storage system logically chunks the storage and keeps track of the status of each chunk(whether its occupied or free)

It handles all storage functionally including allocation, deallocation and writing.

## Storage Chunks

A chunk is a position in the allocaiton map. Its only representation is its index in the allocation map. It functions as the smallest unit of storage addressable by the allocater and de allocator. It is also used for data retreival as its index can be transformed into real storage.

## File System
File system address's storage in logical chunks.

File system is unable to directly access storage addresses. It can only interract with storage through storage manager. It calles storage system functions and refrences desired positions using chunk indices which the storage manager later maps into storage addresses(maping is extremely inexpensive). 

File system manager administers IO, allocation/deallocation and functionality.

It stores meta data about files and folders.

Files are mapped to storage locations using extent-based allocation

## CLI Client
CLI client translates CLI commands into file sys functionality
