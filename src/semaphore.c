#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <immintrin.h>

#include "semaphore.h"

int semaphore_init(Semaphore* sem, int initialValue) {
    if (sem == NULL){
        printf("Null sem given to initialise!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }

    sem->cntThreads = initialValue;                                     // set the initial availability of the semaphore
    return 0;
}

int semaphore_post(Semaphore* sem) {
    if (sem == NULL){
        printf("Null sem given to post!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }

    __sync_fetch_and_add(&(sem->cntThreads), 1);                        // increase the availability of the semaphore
    return 0;
}

int semaphore_wait(Semaphore* sem) {
    if (sem == NULL){
        printf("Null sem given to wait!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }

    while (1) {
        while (sem->cntThreads < 1) {
            _mm_pause();                                                // wait the semaphore to have a lock available
        }
        int newCnt = __sync_add_and_fetch(&(sem->cntThreads), -1);      // try to take the lock
        if (newCnt >= 0) break;                                         // we got the lock
        else __sync_add_and_fetch(&(sem->cntThreads), 1);               // could not take the lock in time, undo the attempt and try again
    }
    return 0;
}

int semaphore_destroy(Semaphore* sem) {
    if (sem == NULL){
        printf("Null sem given to destroy!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }
    sem->cntThreads = 0;
    return 0;
}