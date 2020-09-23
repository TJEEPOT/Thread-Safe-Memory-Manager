/*******************************************************************************

Project : CMP-5013A - Architectures and Operating Systems, Assignment 2:
                      Thread-Safe Memory Manager.

File    : part1.c

Date    : Saturday 30th November 2019

Author  : Martin Siddons

Desc.   : This contains the code for setting up a memory block, dividing it into
          blocks to store given data and then freeing those blocks for later
          allocation.

History : 30/11/2019 - v1.0 - initialise function written and complete.
          02/12/2019 - v1.1 - allocate function finally working correct.
          03/12/2019 - v1.2 - deallocate function completed and tested.

*******************************************************************************/

#include <stdio.h>
#include "part1.h"

void initialise (void *memory, size_t size){
    // form a heap comprising of a single MemBlock node followed by empty space.
    HEAD = memory;
    MemBlock *m = (MemBlock*)HEAD;

    m->allocated = 0;
    m->size = size - MEMBLOCK_SIZE; // MEMBLOCK_SIZE is (originally) 16 bytes.
    m->prev = NULL;
    m->next = NULL;
}

void *allocate (size_t bytes) {
    // iterate through the set of MemBlocks
    MemBlock *curBlock = (MemBlock*)HEAD;
    do{
        if (curBlock->allocated == 0 && curBlock->size >= bytes) {
            // Record how much space is left for future data. We need to ensure
            // everything is aligned correctly from the start, so we align to
            // the size of MemBlock (doing otherwise will cause issues when
            // calculating where curBlock->next points to).
            size_t alignedBytes = bytes + ((sizeof(MemBlock) -
                    bytes % sizeof(MemBlock)) % sizeof(MemBlock));
            long remainingMem = curBlock->size - alignedBytes - MEMBLOCK_SIZE;
            curBlock->allocated = 1; // block is now registered as holding data.
            curBlock->size = alignedBytes; // setting this to alignedBytes
            // rather than bytes to ensure we don't lose memory when
            // deallocating later on.

            // check to ensure there's enough space left to fit another node in.
            if (remainingMem >= 0) {
                // If so, set the address for where the next node will start.
                // The address of the next block must start at the end of the
                // current block alignment.
                size_t MemIncrement = ((alignedBytes + MEMBLOCK_SIZE) /
                                        sizeof(MemBlock));
                curBlock->next = curBlock + MemIncrement;

                MemBlock *nextBlock = curBlock->next;
                nextBlock->allocated = 0;
                nextBlock->size = remainingMem;
                nextBlock->prev = curBlock;
                nextBlock->next = NULL;
            }

            // return the start address of the memory block now allocated.
            return curBlock + 1; // 1 is equal to MEMBLOCK_SIZE bytes.
        }
        curBlock = curBlock->next;
    }
    while(curBlock != NULL);

    return NULL; // If there's insufficient memory space found, return a NULL.
}

void deallocate (void *memory){
    MemBlock *curBlock = (MemBlock*)memory - 1; // assign pointer to a MemBlock
    // strut and decrement the pointer by 1 to align it with the strut in memory
    // rather than the memory space returned by allocate function.
    curBlock->allocated = 0;

    // Check the next MemBlock node to see if it's unallocated.
    MemBlock *nextBlock = curBlock->next;
    if (curBlock->next != NULL && nextBlock->allocated == 0){
        // Not allocated so merge the two blocks, dropping next block.
        curBlock->size = curBlock->size + nextBlock->size + MEMBLOCK_SIZE;
        curBlock->next = nextBlock->next;
    }

    // Also check the previous block to see if that's unallocated too.
    MemBlock *prevBlock = curBlock->prev;
    if (curBlock->prev != NULL && prevBlock->allocated == 0){
        // Not allocated so merge the blocks, dropping current block.
        prevBlock->size = prevBlock->size + curBlock->size + MEMBLOCK_SIZE;
        prevBlock->next = curBlock->next;
    }
}