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

## File Modification System TODO FIX THIS 
File system address's storage in logical chunks.

File system is unable to directly access storage addresses. It can only interract with storage through storage manager. It calles storage system functions and refrences desired positions using chunk indices which the storage manager later maps into storage addresses(maping is extremely inexpensive). 

File system manager administers IO, allocation/deallocation and functionality.

It stores meta data about files and folders.

Files are mapped to storage locations using extent-based allocation

## Compact Extents Sequencing
The order of elements in file.data_chunks_extents indicates the order of how the data is to be read.
compactness is enforced by shifting to the left when a chunk extent is freed. This facilitates append mode

## File Chunk Allocator And Deallocator
file_allocate_chunks and file_free_chunks are abstractions over the storage alloc and dealloc functions challoc and chfree. they call these functions updating a files meta data accordingly 

## File Storage layer
this is the intermediate layer between raw storage IO and file IO. File cannot access storage other than through it. It validates and registers relevent file meta data each time a request is made. 

It enforces file ownership and chunk extent compactness invariants.

however, caller has to explicitly address chunks
## File Logic layer
Abstracts the file storage layer by exposing functions that enable the caller to preform IO tasks on files without having to explicitly address and reason about file storage chunks

Caller is stateless. Each API function here does not depend on the callers state. This, unfortunately, makes the exposed functions verbose.

## RollBack-Based Write Partial Failure Handeling
Since our system relies on and enforces file ownership, such that a chunk can not be accessed by anything other than the file that owns it, failure must be handeled by rolling back, deallocating chunks and modifying file meta-data. This makes for a simpler system but more explicit failure handelling is required as opposed to commit-based partial failure handelling, where chunks are not added into file meta data untill the write processes succeeds.

## duplicate root denotion trade off

## File System Meta Data Store
A structer only in charge of owning the memory storing meta data about files and folders.
It does not enforce or store structer.

## VFS Context
A structer storing a pointer to the file system meta data sore, a pointer to the root directory of the file system and a pointer to the current working directory

The file systems tree heirarchy is implicit from Folder.sub_folders and Folder.sub_files. The base of the heirarchy is the root directory

VFS Context is statefull. It stores the current working directory.

## File System
## Sub entries are different from entries
A sub entrie is created when sub_entry_init is called on the entry and is destroyed when unlink_sub_entry is called
## Node abstraction and the polymorphic trade-off
a node abstraction would have been usefull in shared behaviour that appears in queries and VFS entry store utils ...

Heirarchy is implicit. There isnt an explicit domain in which a node abstraction could be implemented and polymorphisim could be achieved
## why allocation map in VFSEntryStore was chosen in place of vacancy flag within sub entries(vacancy vs non existance)

## Write at does not modify allocated storage. Instead it allocates new


## Even though they reference each other Files and Folders share a relational dependancy. They dont depend on, own or maintain each others life time 

## Paths
explain everything about paths, their design, their invariants and their operations such as resolution

## CLI Client
CLI client translates CLI commands into file sys functionality

## Append oriented systems simplify crashs and reduces fragmentation complexity 

## statefull emulator workspace 
Add convenience layer as it only takes paths not node pointers

## Original I implemented null terminator to indicate the end of a partial chunks data but soon I realised that prevents full byte writing

## input handler
raw mode with posix read()
commands dont own their args the input handler command buffer does
for that purpose the command buffer cannot be reused for data as it would make the command args pointer go stale
thats why there is a seperate buffer for data
 
## tokenizer
in place breaking command into tokens

## parser


## Prompting executor functions
when a command is read if the commands function is not found in the CMD_ARGUMENT_BASED_FUNCTIONS_TABLE the dispatcher searchs for the command function in CMD_PROMPTING_FUNCTIONS_TABLE. If found, the dispatcher calls the mutli_line_read function on the dispatchers data buffer. then the input handlers data buffer is passed to the executor along with the datas length.