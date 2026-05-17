# Todo List

## Appearance
- [X] Add documentation above all exposed Storage functions
- [X] ***Document trade-off between file meta data waste and fragmentation waste.
- [ ] ***Document safety/preformance trade-off between doing a first pass to calculate the datas length before saving 
- [ ] Rewrite design.md
- [ ] Rewrite README.md
- [ ] Refine inline comments accross important functions
- [ ] Add error logging system
- [ ] Add warning to error logging system when chfree is called on a free chunk

## Storage and Storage Management
- [X] Add virtual storage deallocator
- [X] Refactor Storage API
- [X] Add chunk writing function
- [X] Modify return type of challoc to int for status code, and output through pointer out_first_chunk_pos
- [ ] Modify StorageMan.allocation_map to be implemented with bit packing

## TODO
- [X] Abstract the run_test() printf repeatition using pointers to funcions
- [X] Seperate VFS Context and VFS DataStore
- [X] Add cur_path to VFSContext 
- [X] Write global search by name function "find"
- [X] **refactor name_eq it is used everywhere 
- [ ] add "write at" function that doenst actually modify meta data, 
instead it allocates new storage writes into it and the updates meta data to point to the new storage and free the old storage 
- [ ] add "delete at"
- [ ] **add print_tree command
## File Management

## CLI
