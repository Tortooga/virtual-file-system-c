# Virtual file System

## Storage
Virtual disk stored in fixed sized array(contiguous flat storage)

## Storage System
storage_sys logically chunks the storage and keeps track of the status of each chunk(whether its occupied or free)

it also handles allocation and deallocation functionality

## File System 
file_sys manager administers IO, allocation/deallocation and functionality.

It stores meta data about files and folders.

File system address's storage in logical chunks.

Files are mapped to storage locations using extent-based allocation

## CLI Client
CLI client translates CLI commands into file sys functionality
