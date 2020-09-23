/*******************************************************************************

Project : CMP-5013A - Architectures and Operating Systems, Assignment 2:
                      Thread-Safe Memory Manager.

File    : part3.c

Date    : Saturday 07th November 2019

Author  : Martin Siddons

Desc.   : This is a thread-safe version of the implementation defined in
          part2.c.

History : 07/12/2019 - v1.0 - Lock added for allocate to ensure there is no
                              clash between separate allocations.
          11/12/2019 - v1.1 - Lock also shared with initialise function.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "part3.h"

// Macro initialising the mutex to an unlocked state. When called, it will
// lock the designated piece of code to a thread until called to unlock,
// and if the code is locked when it's called, it will yield the thread
// until the locking thread lifts the lock.
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void initialise (void *memory, size_t size, char* algorithm){
    pthread_mutex_lock(&mutex); // lock to ensure the allocation threads don't
    // try to access an uninitialised MemBlock.

    if (algorithm == NULL || strcmp(algorithm, "FirstFit") == 0){
        ALLOCATION_OP = &firstFit;
    }
    else if (strcmp(algorithm, "NextFit") == 0){
        ALLOCATION_OP = &nextFit;
    }
    else if (strcmp(algorithm, "BestFit") == 0){
        ALLOCATION_OP = &bestFit;
    }
    else if (strcmp(algorithm, "WorstFit") == 0){
        ALLOCATION_OP = &worstFit;
    }

    HEAD = memory;
    MemBlock *m = (MemBlock*)HEAD;
    lastBlockPos = NULL;

    // form a heap comprising of a single MemBlock node followed by empty space.
    m->allocated = 0;
    m->size = size - MEMBLOCK_SIZE; // MEMBLOCK_SIZE is (originally) 16 bytes.
    m->prev = NULL;
    m->next = NULL;

    pthread_mutex_unlock(&mutex); // unlock the mutex to allow allocation
}

void *allocate (size_t bytes) {
    MemBlock *firstBlock = (MemBlock*)HEAD; // assign the start of the memory
    // block as holding details about the first node.
    void *(*fp) (size_t, MemBlock*); // define the function pointer.
    fp = ALLOCATION_OP; // assign fp the operation decided in initialise.

    pthread_mutex_lock(&mutex); // lock allocation to a single thread.
    MemBlock *curBlock = fp(bytes, firstBlock); // run the assigned function to
    // find the position of the next block to allocate to.
    if (curBlock == NULL){
        pthread_mutex_unlock(&mutex); // unlock the mutex before quitting.
        return NULL;
    }

    // With the block found, allocate it with the code below.
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
    // deallocating later on. It doesn't hurt to give a little more
    // space than what was requested.

    // check to ensure there's enough space left to fit another node in.
    if (remainingMem >= 0) {
        // If so, set the address for where the next node will start.
        // The address of the next block must start at the end of the
        // current block alignment.
        size_t MemIncrement = (alignedBytes + MEMBLOCK_SIZE) /
                              sizeof(MemBlock);
        curBlock->next = curBlock + MemIncrement;

        MemBlock *nextBlock = curBlock->next;
        nextBlock->allocated = 0;
        nextBlock->size = remainingMem;
        nextBlock->prev = curBlock;
        nextBlock->next = NULL;
    }

    // now the block has been allocated, remove the mutex.
    pthread_mutex_unlock(&mutex);

    // return the start address of the memory block now allocated.
    return curBlock + 1; // 1 is equal to MEMBLOCK_SIZE bytes.
}

void *firstFit (size_t bytes, MemBlock *curBlock){
    // Iterate through the set of MemBlocks starting from the first block.
    do{
        if (curBlock->allocated == 0 && curBlock->size >= bytes) {
            return curBlock;
        }
        curBlock = curBlock->next;
    }
    while(curBlock != NULL);

    return NULL; // If there's insufficient memory space found, return a NULL.
}

void *nextFit (size_t bytes, MemBlock *firstBlock){
    // Assign the first node as the last node looked at (if there is one).
    MemBlock *curBlock = firstBlock;
    if (lastBlockPos != NULL){
        curBlock = lastBlockPos;
    }

    // The rest of the allocation is carried out the same as firstFit.
    do{
        if (curBlock->allocated == 0 && curBlock->size >= bytes) {
            lastBlockPos = curBlock;
            return curBlock;
        }
        curBlock = curBlock->next;
    }
    while(curBlock != NULL);

    // If the end of the allocated memory block is reached and we started
    // from mid-way through the set, loop back and look from the start.
    if (lastBlockPos != NULL && lastBlockPos != firstBlock){
        curBlock = firstBlock;
        do{
            if (curBlock->allocated == 0 && curBlock->size >= bytes) {
                lastBlockPos = curBlock;
                return curBlock;
            }
            curBlock = curBlock->next;
        }
        while(curBlock != lastBlockPos);
    }

    return NULL; // If there's insufficient memory space found, return a NULL.
}

void *bestFit (size_t bytes, MemBlock *curBlock){
    size_t closestSize = -1; // store the value of the closest-sized free block
    MemBlock *closestBlock = NULL; // and the address of the closest block

    // Find a suitable-sized block by iterating through the entire memory space.
    do{
        if (curBlock->allocated == 0) {
            if (curBlock->size > bytes && curBlock->size < closestSize){
                closestSize = curBlock->size;
                closestBlock = curBlock;
            }
            else if (curBlock->size == bytes){
                return curBlock; // if the perfect block is found, return that.
            }
        }
        curBlock = curBlock->next;
    }
    while(curBlock != NULL);

    // Once the list has been iterated through, return the address of the
    // smallest block that will fit the size requirement. This will be NULL if
    // the block could not be allocated.
    return closestBlock;
}

void *worstFit (size_t bytes, MemBlock *curBlock){
    size_t largestSize = 0; // store the value of the largest-sized free block
    MemBlock *largestBlock = NULL; // and the address of the largest block

    // Find the largest block by iterating through the memory space list.
    do{
        if (curBlock->allocated == 0) {
            if (curBlock->size > bytes && curBlock->size > largestSize){
                largestSize = curBlock->size;
                largestBlock = curBlock;
            }
        }
        curBlock = curBlock->next;
    }
    while(curBlock != NULL);

    // Once the list has been iterated through, return the address of the
    // smallest block that will fit the size requirement. This will be NULL if
    // the block could not be allocated.
    return largestBlock;
}


void deallocate (void *memory){
    pthread_mutex_lock(&mutex); // lock to ensure allocation has been finished

    if (memory == NULL){
        printf("Unable to deallocate a null pointer.\n");
        pthread_mutex_unlock(&mutex); // unlock and continue.
        return;
    }

    MemBlock *curBlock = (MemBlock*)memory - 1; // assign pointer to a MemBlock
    // strut and decrement the pointer by 1 to align it with the strut in memory
    // rather than the memory space returned by allocate function.
    curBlock->allocated = 0;

    // Check the next MemBlock node to see if it's unallocated.
    MemBlock *nextBlock = curBlock->next;
    if (curBlock->next != NULL && nextBlock->allocated == 0){
        // Not allocated so merge the two blocks, dropping next block.a
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

    pthread_mutex_unlock(&mutex); // allow threads to allocate to freed memory
}


void *worker(void *wsPointer){
    Workspace *workspace = (Workspace*)wsPointer;
    workspace->addr = allocate(workspace->bytes);
    return NULL;
}