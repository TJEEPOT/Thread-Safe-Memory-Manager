/*******************************************************************************

Project : CMP-5013A - Architectures and Operating Systems, Assignment 2:
                      Thread-Safe Memory Manager.

File    : part3.h

Date    : Saturday 07th November 2019

Author  : Martin Siddons

Desc.   : Definitions and further information for thread-safe version of
          implementation defined in part2.h, which is used in part3.c.

History : 07/12/2019 - v1.0 -
          xx/12/2019 - v1.1 -

*******************************************************************************/

#ifndef MEMMAN3_PART3_H
#define MEMMAN3_PART3_H

// Struct definitions
typedef struct MemBlock{
    int allocated;          // 0 if empty, 1 if allocated
    size_t size;            // size of the data segment after this strut
    struct MemBlock *prev;  // pointer to the start of the previous MemBlock
    struct MemBlock *next;  // pointer to the start of the next MemBlock
} MemBlock;

typedef struct WorkspaceStruct{
    size_t bytes;
    void *addr;
} Workspace;

#define MEMBLOCK_SIZE sizeof(MemBlock)
const void *HEAD; // declare a pointer to the start of the assigned memory space
MemBlock *lastBlockPos; // pointer to the last allocated block used
void *ALLOCATION_OP; // declare the allocation function to be used

// Initialise is similar to part1.c, however it now also takes a string with the
// name of the algorithm to be used for allocation within it.
void initialise (void *memory, size_t size, char* algorithm);


// Allocate is now a function pointer to 4 different algorithms which are chosen
// based on the value held in ALLOCATION_OP char. Pass the given size
// of memory to the specified allocation operation below.
void *allocate (size_t bytes);

// Function used by allocate for allocating via First Fit. This is exactly the
// same operation as the allocate function performs in part1.
void *firstFit (size_t bytes, MemBlock *curBlock);

// Function for allocating via Next Fit, which will recall the address of the
// last block allocated and start checking from there. The bulk of the
// allocation is done via firstFit once the starting position has been found.
void *nextFit (size_t bytes, MemBlock *curBlock);

// Best Fit allocation algorithm. This looks through the entire memory space
// looking for a hole the same size as the block to allocate. If this isn't
// found, it will allocate to the smallest-sized block larger than the requested
// size.
void *bestFit (size_t bytes, MemBlock *curBlock);

// Worst Fit algorithm, used by allocate. This works similar to bestFit but
// iterates through the list to find the largest block of memory larger than the
// one requested, then allocates to that block.
void *worstFit (size_t bytes, MemBlock *curBlock);
// End of allocation functions.


// Deallocate, same as part1.h
void deallocate (void *memory);


// Worker function. This is passed into pthread_create (as the start_routine
// function pointer) and defines the start of the thread's execution. It is
// defined as returning a void pointer even though the return value will always
// be NULL, as pthread_create would be unable to use it otherwise.
void *worker(void *wsPointer);

#endif