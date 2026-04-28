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
- [ ] Abstract the run_test() printf repeatition using pointers to funcions

## File Management

## CLI
