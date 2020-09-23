/*******************************************************************************

Project : CMP-5013A - Architectures and Operating Systems, Assignment 2:
                      Thread-Safe Memory Manager.

File    : part2.h

Date    : Saturday 30th November 2019

Author  : Martin Siddons

Desc.   : Definition for memory block node and further information for functions
          used in part2.c basic structure can be compared to part1.h.

History : 05/12/2019 - v1.0 - Set up all part2 function names.
          06/12/2019 - v1.1 - Adjusted function variables and completed comments
            for all functions.

*******************************************************************************/

#ifndef MEMMAN_PART2_H
#define MEMMAN_PART2_H

// Struct definition
typedef struct MemBlock{
    int allocated;          // 0 if empty, 1 if allocated
    size_t size;            // size of the data segment after this strut
    struct MemBlock *prev;  // pointer to the start of the previous MemBlock
    struct MemBlock *next;  // pointer to the start of the next MemBlock
} MemBlock;

#define MEMBLOCK_SIZE sizeof(MemBlock)
const void *HEAD; // declare a pointer to the start of the assigned memory space
MemBlock *lastBlockPos; // pointer to the last allocated block used
void *ALLOCATION_OP; // declare the allocation function to be used

// Initialise is similar to part1.c, however it now also takes a string with the
// name of the allocation algorithm to be used within it. When matched, this
// algorithm name saves the address of the subsequent matching function to
// ALLOCATION_OP, above.
void initialise (void *memory, size_t size, char* algorithm);

// Allocate now holds a function pointer to 4 different algorithms which are
// chosen based on the value held in ALLOCATION_OP char. Pass the given size
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


// Deallocate, this is the same as defined in part1.h.
void deallocate (void *memory);

#endif