/*******************************************************************************

Project : CMP-5013A - Architectures and Operating Systems, Assignment 2:
                      Thread-Safe Memory Manager.

File    : part3_test.c

Date    : Saturday 07th November 2019

Author  : Martin Siddons

Desc.   : This is the test harness for the functions defined in part3.h. The
          tests are designed to be the same as the ones defined in part2_test.c
          but working with threads instead.

History : 07/12/2019 - v1.0 - Separated out test harness from part3.c

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "part3.h"

int threadCheck(int threadid){
    if (threadid != 0){
        perror("pthread_create error.");
        return EXIT_FAILURE;
    }
}

int main() {
    size_t memSize = 0xff; // set to allocate 256 bytes of memory for testing
    int ffTests = 1,     // define how many tests we are doing of FirstFit.
            nfTests = 5, // number of NextFit tests.
            bfTests = 7, // BestFit tests.
            wfTests = 7, // WorstFit tests.
            tid;         // thread id (for checking pthread_create errors).

    // Test FirstFit to ensure it is actually still allocating correctly.
    // First set up memory structures for test workers and workspaces.
    pthread_t *ffWorkers   = (pthread_t*)calloc(ffTests, sizeof(pthread_t));
    Workspace *ffWorkspace = (Workspace*)calloc(ffTests, sizeof(Workspace));

    void *firstFitAlloc = malloc(memSize);
    initialise(firstFitAlloc, memSize, "FirstFit");

    ffWorkspace[0].bytes = 100;
    tid = pthread_create(&ffWorkers[0], NULL, worker, &ffWorkspace[0]);
    threadCheck(tid);

    for(int i = 0; i < ffTests; i++){
        pthread_join(ffWorkers[i], NULL);
    }
    printf("FirstFit 0 start address: %p\n\n", ffWorkspace[0].addr);

    free(firstFitAlloc);
    free(ffWorkspace);
    free(ffWorkers);


    // Test the functionality of NextFit.
    // Memory structures for testing:
    pthread_t *nfWorkers   = (pthread_t*)calloc(nfTests, sizeof(pthread_t));
    Workspace *nfWorkspace = (Workspace*)calloc(nfTests, sizeof(Workspace));

    void *nextFitAlloc = malloc(memSize);
    initialise(nextFitAlloc, memSize, "NextFit");

    nfWorkspace[0].bytes = 100;
    tid = pthread_create(&nfWorkers[0], NULL, worker, &nfWorkspace[0]);
    threadCheck(tid);

    nfWorkspace[1].bytes = 40;
    tid = pthread_create(&nfWorkers[1], NULL, worker, &nfWorkspace[1]);
    threadCheck(tid);

    pthread_join(nfWorkers[0], NULL);
    printf("NextFit 0 start address:  %p\n", nfWorkspace[0].addr);
    pthread_join(nfWorkers[1], NULL);
    printf("NextFit 1 start address:  %p\n", nfWorkspace[1].addr);

    deallocate(nfWorkspace[0].addr);

    nfWorkspace[2].bytes = 40;
    tid = pthread_create(&nfWorkers[2], NULL, worker, &nfWorkspace[2]);
    threadCheck(tid); // should allocate after nextFit[1]

    nfWorkspace[3].bytes = 40;
    tid = pthread_create(&nfWorkers[3], NULL, worker, &nfWorkspace[3]);
    threadCheck(tid); // should allocate in place of nextFit[0]

    nfWorkspace[4].bytes = 60;
    tid = pthread_create(&nfWorkers[4], NULL, worker, &nfWorkspace[4]);
    threadCheck(tid); // should not allocate

    pthread_join(nfWorkers[2], NULL);
    printf("NextFit 2 start address:  %p\n", nfWorkspace[2].addr);
    pthread_join(nfWorkers[3], NULL);
    printf("NextFit 3 start address:  %p\n", nfWorkspace[3].addr);
    pthread_join(nfWorkers[4], NULL);
    printf("NextFit 4 start address:  %p\n\n", nfWorkspace[4].addr);

    free(nextFitAlloc);
    free(nfWorkspace);
    free(nfWorkers);


    // Test the functionality of BestFit.
    // Memory structures for testing:
    pthread_t *bfWorkers   = (pthread_t*)calloc(bfTests, sizeof(pthread_t));
    Workspace *bfWorkspace = (Workspace*)calloc(bfTests, sizeof(Workspace));

    void *bestFitAlloc = malloc(memSize);
    initialise(bestFitAlloc, memSize, "BestFit");

    bfWorkspace[0].bytes = 32;
    tid = pthread_create(&bfWorkers[0], NULL, worker, &bfWorkspace[0]);
    threadCheck(tid);

    bfWorkspace[1].bytes = 32;
    tid = pthread_create(&bfWorkers[1], NULL, worker, &bfWorkspace[1]);
    threadCheck(tid);

    bfWorkspace[2].bytes = 16;
    tid = pthread_create(&bfWorkers[2], NULL, worker, &bfWorkspace[2]);
    threadCheck(tid);

    bfWorkspace[3].bytes = 32;
    tid = pthread_create(&bfWorkers[3], NULL, worker, &bfWorkspace[3]);
    threadCheck(tid);

    pthread_join(bfWorkers[0], NULL);
    printf("bestFit 0 start address:  %p\n", bfWorkspace[0].addr);
    pthread_join(bfWorkers[1], NULL);
    printf("bestFit 1 start address:  %p\n", bfWorkspace[1].addr);
    pthread_join(bfWorkers[2], NULL);
    printf("bestFit 2 start address:  %p\n", bfWorkspace[2].addr);
    pthread_join(bfWorkers[3], NULL);
    printf("bestFit 3 start address:  %p\n", bfWorkspace[3].addr);

    deallocate(bfWorkspace[0].addr);
    deallocate(bfWorkspace[2].addr);

    bfWorkspace[4].bytes = 16;
    tid = pthread_create(&bfWorkers[4], NULL, worker, &bfWorkspace[4]);
    threadCheck(tid); // will go in place of bestFit[2]

    bfWorkspace[5].bytes = 26;
    tid = pthread_create(&bfWorkers[5], NULL, worker, &bfWorkspace[5]);
    threadCheck(tid); // will go in place of bestFit[0]

    bfWorkspace[6].bytes = 100;
    tid = pthread_create(&bfWorkers[6], NULL, worker, &bfWorkspace[6]);
    threadCheck(tid); // will not allocate

    pthread_join(bfWorkers[4], NULL);
    printf("bestFit 4 start address:  %p\n", bfWorkspace[4].addr);
    pthread_join(bfWorkers[5], NULL);
    printf("bestFit 5 start address:  %p\n", bfWorkspace[5].addr);
    pthread_join(bfWorkers[6], NULL);
    printf("bestFit 6 start address:  %p\n\n", bfWorkspace[6].addr);

    free(bestFitAlloc);
    free(bfWorkspace);
    free(bfWorkers);


    // Test the functionality of WorstFit.
    // Memory structures for testing:
    pthread_t *wfWorkers   = (pthread_t*)calloc(wfTests, sizeof(pthread_t));
    Workspace *wfWorkspace = (Workspace*)calloc(wfTests, sizeof(Workspace));

    void *worstFitAlloc = malloc(memSize);
    initialise(worstFitAlloc, memSize, "WorstFit");

    wfWorkspace[0].bytes = 28;
    tid = pthread_create(&wfWorkers[0], NULL, worker, &wfWorkspace[0]);
    threadCheck(tid);

    wfWorkspace[1].bytes = 32;
    tid = pthread_create(&wfWorkers[1], NULL, worker, &wfWorkspace[1]);
    threadCheck(tid);

    wfWorkspace[2].bytes = 16;
    tid = pthread_create(&wfWorkers[2], NULL, worker, &wfWorkspace[2]);
    threadCheck(tid);

    wfWorkspace[3].bytes = 32;
    tid = pthread_create(&wfWorkers[3], NULL, worker, &wfWorkspace[3]);
    threadCheck(tid);

    pthread_join(wfWorkers[0], NULL);
    printf("worstFit 0 start address: %p\n", wfWorkspace[0].addr);
    pthread_join(wfWorkers[1], NULL);
    printf("worstFit 1 start address: %p\n", wfWorkspace[1].addr);
    pthread_join(wfWorkers[2], NULL);
    printf("worstFit 2 start address: %p\n", wfWorkspace[2].addr);
    pthread_join(wfWorkers[3], NULL);
    printf("worstFit 3 start address: %p\n", wfWorkspace[3].addr);

    deallocate(wfWorkspace[0].addr);
    deallocate(wfWorkspace[2].addr);

    wfWorkspace[4].bytes = 32;
    tid = pthread_create(&wfWorkers[4], NULL, worker, &wfWorkspace[4]);
    threadCheck(tid); // will go after worstFit [3]

    wfWorkspace[5].bytes = 32;
    tid = pthread_create(&wfWorkers[5], NULL, worker, &wfWorkspace[5]);
    threadCheck(tid); // will go in place of worstFit [0]

    wfWorkspace[6].bytes = 32;
    tid = pthread_create(&wfWorkers[6], NULL, worker, &wfWorkspace[6]);
    threadCheck(tid); // no space for this, will return NULL

    pthread_join(wfWorkers[4], NULL);
    printf("worstFit 4 start address: %p\n", wfWorkspace[4].addr);
    pthread_join(wfWorkers[5], NULL);
    printf("worstFit 5 start address: %p\n", wfWorkspace[5].addr);
    pthread_join(wfWorkers[6], NULL);
    printf("worstFit 6 start address: %p\n\n", wfWorkspace[6].addr);

    free(worstFitAlloc);
    free(wfWorkspace);
    free(wfWorkers);

    return EXIT_SUCCESS;
}