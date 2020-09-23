/*******************************************************************************

Project : CMP-5013A - Architectures and Operating Systems, Assignment 2:
                      Thread-Safe Memory Manager.

File    : part1_test.c

Date    : Saturday 30th November 2019.

Author  : Martin Siddons

Desc.   : This is the test harness for part1.c to ensure the included functions
          work correctly.

History : 11/12/2019 - v1.0 - Separated out test structure from part1.c.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "part1.h"

int main() {
    size_t memSize = 0xff; // allocate 256 bytes of memory for testing
    void *m = malloc(memSize);
    initialise(m, memSize);

    void *firstBlock  = allocate(100);
    void *secondBlock = allocate(61);
    void *thirdBlock  = allocate(64); // should be too big to allocate
    void *fourthBlock = allocate(16);

    printf("Memory Block start address:  %p\n\n", m);
    printf("First memory start address:  %p\n", firstBlock);
    printf("Second memory start address: %p\n", secondBlock);
    printf("Third memory start address:  %p\n", thirdBlock);
    printf("Fourth memory start address: %p\n\n", fourthBlock);

    deallocate(secondBlock);
    void *fifthBlock = allocate(70); // should be too big
    void *sixthBlock = allocate(10); // will allocate in place of
    // secondBlock

    printf("Fifth memory start address: %p\n", fifthBlock);
    printf("Sixth memory start address: %p\n\n", sixthBlock);

    free(m);
    return EXIT_SUCCESS;
}