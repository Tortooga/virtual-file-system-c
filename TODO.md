# Todo List

## Appearance
- [ ] Add documentation above all exposed functions
- [ ] Rewrite design.md
- [ ] Rewrite README.md
- [ ] Refine inline comments accross important functions
- [ ] Add error logging system
- [ ] Add warning to error logging system when chfree is called on a free chunk
## Storage and Storage Management
- [X] Add virtual storage deallocator
- [ ] Modify StorageMan.allocation_map to be implemented with bit packing
- [ ] Refactor Storage API
- [X] Add chunk writing function
- [X] Modify return type of challoc to int for status code, and output through pointer out_first_chunk_pos
- [ ] Change magic error codes into enums
## File Management

## CLI
