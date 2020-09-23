/*******************************************************************************

Project : CMP-5013A - Architectures and Operating Systems, Assignment 2:
                      Thread-Safe Memory Manager.

File    : part1.h

Date    : Saturday 30th November 2019

Author  : Martin Siddons

Desc.   : Definition for memory block node and further information for functions
          implemented in part1.c.

History : 30/11/2019 - v1.0 - Put together MemBlock Strut and wrote initial defs
          02/12/2019 - v1.1 - Completed task #1 exercise descriptions.

*******************************************************************************/

#ifndef MEMMAN_PART1_H
#define MEMMAN_PART1_H

// Struct definition
typedef struct MemBlock{
    int allocated;          // 0 if empty, 1 if allocated
    size_t size;            // size of the data segment after this strut
    struct MemBlock *prev;  // pointer to the start of the previous MemBlock
    struct MemBlock *next;  // pointer to the start of the next MemBlock
} MemBlock;

#define MEMBLOCK_SIZE sizeof(MemBlock)
const void *HEAD; // declare a pointer to the start of the assigned memory space


// Set up a MemBlock as a Heap in the space given by *memory and size. The
// address of *memory is saved to the value HEAD, defined above, so allocate and
// deallocate functions know where the first MemBlock strut starts from.
void initialise (void *memory, size_t size);

// Find the next block of memory in the initialised heap large enough to store
// the MemBlock and the requested data defined in the function variable 'bytes'.
// Return the address of the start of this allocated memory block, or NULL if a
// block of the requested size does not exist.
void *allocate (size_t bytes);

// Allow the MemBlock at the address passed in as the pointer *memory and
// associated data to be overwritten. Also check if nearby blocks are also empty
// and if so, it will merge them together.
void deallocate (void *memory);

#endif