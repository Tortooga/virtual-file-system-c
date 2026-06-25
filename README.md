# **Virtual File System**

## **Table of Contents**
- [Introduction](#introduction)
- [System Overview](#system-overview)
- [Workspace Overview](#workspace-overview)
- [CLI Overview](#cli-overview)
- [Control Flow](#control-flow)
- [Tradeoffs](#tradeoffs)
- [Limitations](#limitations)



## **Introduction**
This is a file system abstraction layer with extent-based allocation and IO over a chunk addressed heap allocated virtual disk.

The file system depends solely on a custom virtual storage interface, making it agnostic of the storage backend. In the current implementation, the virtual storage interface is implemented over an in-memory virtual disk.



## **System Overview**
This systems architecuter is layered and stateless. The Following Is a high-level characterisation of the systems layers.

### **Storage(Virtual Disk)**
The storage is a contiguous sequence of bytes in the heap.
It is of constant size throughout the systems runtime. There is no physical enforcement or indication of chunking at this layer.

### **Storage Management**
this layer serves as the only interface between the file system and the storage system. It exposes storage I/O and storage allocator primitives.

#### *Storage chunks*
A chunk is the smallest allocatable unit of storage. All storage chunks are of the same size. The functions exposed at this layer take chunk index arguments (the position of a chunk in the contiguous storage) as the only way to address specific regions. They then internally convert those chunks into actual storage addresses. This gives the illusion that the storage is physically chunked even though it is merely logically chunked.


#### *Storage Manager*
The storage manager, commonly refered to as *storage_man*, owns the storage meta data, such as the allocation status of each storage chunk.

Respect of allocation -the fact that an occupied chunk can't be allocated and a free chunk can't be accessed- is enforced in this layer.

### **Files**
Files are objects that own locations in storage. Files store meta data about their allocated storage locations in *chunk extents*. That is, they are allocated contigous sequences in storage which they then keep track of by storing the start location and amount of bytes in each sequence (see [File Meta Data Overhead](DONT-FORGET-TO-ADD-LINK)). Each file belongs to a parent (see [Folders](#folders)).

The File I/O is split into the following two layers:

#### 1. File Storage
this layer is responsible for using the storage manager to perform allocation and I/O operations on a file. the functions exposed at this layer allow the caller to address storage using chunk extents, which allows them to address storage regions of arbitrary width. After which it enforces file ownership on the specified storage, only allowing a file to access the chunks it owns. Then it methodically calls the storage manager-which can only reason about single chunks- on each of the addressed chunks.


#### 2. File Logic 
This layer encapsulates all storage details. Its exposed functions do not require the caller to reason about allocation, storage locations, or the storage manager. The caller merely provides highlevel information about their desired operation, such as a data buffer if the caller wishes to perform a write operation.

Even when performing a positional read operation, the caller does not pass storage chunk info. Instead, the caller passes relative byte information that indicates the datas sudo-position within the file. This, most of the time, does not reflect the actual data's structer in storage.

### **System Hierarchy**
At this point, there is no way to operate on a file(and in turn operate on storage) without directly having the files address in memroy. This layer introduces a hierarchical ecosystem in which files and folders have unique identifiers by which they can be referenced.

#### *Folders*
A folder is an object with a parent folder(required) and a collection of sub files and sub folders (folders are self referential). The root is a special kind of folder that does not have a parents, it serves the purpose of being the origin of the file system. Folders function as a way to give a collection of files structure.


#### *VFSEntryStore*
The VFSEntryStore is a flat collection of file and folder memory locations. All the entries(files and folders) in the file system are stored here. When an entry is created a special allocator allocates it space in the VFSEntryStore. And when an entry is deleted, its position is freed for future use. The VFSEntryStore is not aware of the structure of the file system, it is only responsible for storing the entries regardless of how they may be structured. The files systems structure, instead, is given by the parent and child relations between the entries.

#### *Paths and Queries*
The aforementioned unqiue identifier for each entry is the entry path. In this system, local name collision is strictly forbidden. That is, no two entries under the same folder can have the same name. This makes it so that each entry has a globaly unqiue path(the sequence of an entries parents and its name). Queries use this property to retreave an entry using its identifier or path. It is possible, for example, to retrieve an entry under a certain folder using its name(local entry search by name). It is also possible to retrieve it by using its path(path resolution). If an entries parent is unknown, the system also implements a global entry search by name function(find), but this may return multiple entries(distinguished by path) as there is nothing that prevents global identifier collision(a classic design for most file systems).

Hence, the system heirarchy layer provides a way to store, structure and retreive entries of the file system.


## **Workspace Overview**
As we mentioned earlier, the core [system](#system-overview) is stateless. It has no concept of user specific context such as  the current working directory.
The *workspace* is not a part of the core system, rather it is a user facing stateful wrapper of the [system](#system-overview). It simplifies the [system](#system-overview)'s interface by using information stored in its context. It also encapsulates the internal representation of entries by only allowing entries to be addressed using their names or paths. This layer is intended as a more concise but less versitile interface to the core [system](#system-overview). 


## **CLI Overview**
The CLI is implemented over the [workspace](#workspace-overview). It is broken down into a sequence of steps starting by reading the command from the terminal and ending by performing the correct [workspace](#workspace-overview) call and showing the correct output. The following is a high-level overview of this processes.

### **Input Handling**
The input handler is implemented on canonical mode. It uses the POSIX syscall read() to stream input from the terminal buffer. It has two internal modes; Multiline mode(EOF terminated reading, input is processed when user sends an EOF signal using CTRL + 'D') and Newline terminated mode(input is processed when user submits '\n').

### **Tokenisation**
Once the input is submitted the tokeniser tokenises it in place. It places null terminators in place of token delimiters and stores the address of the start of each token.


### **Command Parser**
The command parser parses tokenised input into command objects.

### **Command Dispatcher**
The command dispatcher accepts command objects and calls the appropriate [command executor](command-execution) passing in the command arguments and options

### **Command Execution**
Command Executors accept command arguments and options, using the workspace interface to perform system operations. Each command executor also handles its commands output.

## Control Flow
The following is a diagram demonstrating the control flow of the entire system

[Control Flow Diagram](docs/VFS_Architecture_Diagram.svg)