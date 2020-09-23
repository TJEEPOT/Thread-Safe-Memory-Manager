/*******************************************************************************

Project : CMP-5013A - Architectures and Operating Systems, Assignment 2:
                      Thread-Safe Memory Manager.

File    : part2_test.c

Date    : Saturday 30th November 2019

Author  : Martin Siddons

Desc.   : This file contains the test harness for part2.c, testing anything that
          has been altered from part1.c.

History : 11/12/2019 - v1.0 - Separated out test harness from part2.c.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "part2.h"

int main() {
    size_t memSize = 0xff; // set to allocate 256 bytes of memory for testing

    // Test FirstFit to ensure it is actually still allocating correctly.
    void *firstFitAlloc = malloc(memSize);
    initialise(firstFitAlloc, memSize, "FirstFit");
    void *firstFit1  = allocate(100);
    printf("FirstFit block 1 start address: %p\n\n", firstFit1);
    free(firstFitAlloc);

    // Test the functionality of NextFit.
    void *nextFitAlloc = malloc(memSize);
    initialise(nextFitAlloc, memSize, "NextFit");
    void *nextFit1 = allocate(100);
    printf("NextFit block 1 start address:  %p\n", nextFit1);
    void *nextFit2 = allocate(40);
    printf("NextFit block 2 start address:  %p\n", nextFit2);
    deallocate(nextFit1);
    void *nextFit3 = allocate(40); // should allocate after nextFit2
    printf("NextFit block 3 start address:  %p\n", nextFit3);
    void *nextFit4 = allocate(40); // should allocate in place of nextFit1
    printf("NextFit block 4 start address:  %p\n", nextFit4);
    void *nextFit5 = allocate(60); // should not allocate
    printf("NextFit block 5 start address:  %p\n\n", nextFit5);
    free(nextFitAlloc);

    // Test the functionality of BestFit.
    void *bestFitAlloc = malloc(memSize);
    initialise(bestFitAlloc, memSize, "BestFit");
    void *bestFit1 = allocate(32);
    printf("bestFit block 1 start address:  %p\n", bestFit1);
    void *bestFit2 = allocate(32);
    printf("bestFit block 2 start address:  %p\n", bestFit2);
    void *bestFit3 = allocate(16);
    printf("bestFit block 3 start address:  %p\n", bestFit3);
    void *bestFit4 = allocate(32);
    printf("bestFit block 4 start address:  %p\n", bestFit4);
    deallocate(bestFit1);
    deallocate(bestFit3);
    void *bestFit5 = allocate(16); // will go in place of bestFit3
    printf("bestFit block 5 start address:  %p\n", bestFit5);
    void *bestFit6 = allocate(26); // will go in place of bestFit1
    printf("bestFit block 6 start address:  %p\n", bestFit6);
    void *bestFit7 = allocate(100); // will not allocate
    printf("bestFit block 7 start address:  %p\n\n", bestFit7);
    free(bestFitAlloc);

    // Test the functionality of WorstFit.
    void *worstFitAlloc = malloc(memSize);
    initialise(worstFitAlloc, memSize, "WorstFit");
    void *worstFit1 = allocate(28);
    printf("worstFit block 1 start address: %p\n", worstFit1);
    void *worstFit2 = allocate(32);
    printf("worstFit block 2 start address: %p\n", worstFit2);
    void *worstFit3 = allocate(16);
    printf("worstFit block 3 start address: %p\n", worstFit3);
    void *worstFit4 = allocate(32);
    printf("worstFit block 4 start address: %p\n", worstFit4);
    deallocate(worstFit1);
    deallocate(worstFit3);
    void *worstFit5 = allocate(32); // will go after worstFit4
    printf("worstFit block 5 start address: %p\n", worstFit5);
    void *worstFit6 = allocate(16); // will go in place of worstFit1
    printf("worstFit block 6 start address: %p\n", worstFit6);
    void *worstFit7 = allocate(90); // no space for this, will return NULL
    printf("worstFit block 7 start address: %p\n\n", worstFit7);
    free(worstFitAlloc);

    return EXIT_SUCCESS;
}