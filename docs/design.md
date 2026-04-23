# Virtual file System

## Storage
Virtual disk stored in fixed sized array(contiguous flat storage)

## Storage System
storage_sys logically chunks the storage and keeps track of the status of each chunk(whether its occupied or free)

it also handles allocation and deallocation functionality

## Storage Chunks

A chunk is a position in the allocaiton map. Its only representation is its index in the allocation map. It functions as the smallest unit of storage addressable by the allocater and de allocator. It is also used for data retreival as its index can be transformed into real storage.

## File System

File System is unable to directly access storage addresses. It can only refrence storage positions using chunk indexes.
 
file_sys manager administers IO, allocation/deallocation and functionality.

It stores meta data about files and folders.

File system address's storage in logical chunks.

Files are mapped to storage locations using extent-based allocation

## CLI Client
CLI client translates CLI commands into file sys functionality
