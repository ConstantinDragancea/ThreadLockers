#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <immintrin.h>

#include "semaphore_slow.h"

int semaphore_slow_init(Semaphore_Slow* sem, int initialValue) {
    if (sem == NULL){
        printf("Null sem given to initialise!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }

    sem->cntThreads = initialValue;                                     // set the initial availability of the semaphore
    return 0;
}

int semaphore_slow_post(Semaphore_Slow* sem) {
    if (sem == NULL){
        printf("Null sem given to post!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }

    __sync_fetch_and_add(&(sem->cntThreads), 1);                        // increase the availability of the semaphore
    return 0;
}

int semaphore_slow_wait(Semaphore_Slow* sem) {
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
        if (newCnt >= 0) return 0;                                      // we got the lock
        else __sync_add_and_fetch(&(sem->cntThreads), 1);               // could not take the lock in time, undo the attempt and try again
    }
    return 0;
}

int semaphore_slow_destroy(Semaphore_Slow* sem) {
    if (sem == NULL){
        printf("Null sem given to destroy!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }
    sem->cntThreads = 0;
    return 0;
}