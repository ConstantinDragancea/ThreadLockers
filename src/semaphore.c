#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <immintrin.h>

#include "semaphore.h"

void signal_handler_semaphore(int sig) {
    // printf("Thread %d woke up!\n", pthread_self());
    return;
}

int semaphore_init(Semaphore* sem, int initialValue) {
    if (sem == NULL){
        printf("Null sem given to initialise!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }

    signal(SIGUSR1, signal_handler_semaphore);

    // set the initial availability of the semaphore
    sem->cntThreads = initialValue;

    sem->guard = false;
    sem->thr_queue = malloc(sizeof(Thread_Queue));
    TQueue_Init(sem->thr_queue);
    return 0;
}

int semaphore_post(Semaphore* sem) {
    if (sem == NULL){
        printf("Null sem given to post!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }

    // Try to take the guard lock for the queue
    while (1) {
        bool value = __sync_val_compare_and_swap(&(sem->guard), false, true);
        if (value == false) {
            break;
        }
    }

    //TQueue_Print(sem->thr_queue);
    // Wake up the first thread from the queue
    pthread_t* to_wake_up = TQueue_Pop(sem->thr_queue);
    if (to_wake_up != NULL) {

        // send signal to the thread
        if (pthread_kill(*to_wake_up, SIGUSR1) != 0){
            printf("Couldn't wake up thread!\n");
            char message[] = "Error message: ";
            perror(message);
        }
        
        //printf("Woke up thread %d\n", *to_wake_up);
    }

    // increase the availability of the semaphore
    __sync_fetch_and_add(&(sem->cntThreads), 1);

    // release the guard lock 
    __sync_val_compare_and_swap(&(sem->guard), true, false);
    
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

        // Try to take the guard lock for the queue
        while (1) {
            bool value = __sync_val_compare_and_swap(&(sem->guard), false, true);
            if (value == false) {
                break;
            }
        }

        // try to take a lock in the semaphore
        int newCnt = __sync_add_and_fetch(&(sem->cntThreads), -1);

        if (newCnt >= 0) {
            // we got the lock
            // release the guard lock 
            __sync_val_compare_and_swap(&(sem->guard), true, false);
            return 0;
        }

        // Could not take the lock from the semaphore, undo the attempt, insert in queue and go to sleep
        __sync_add_and_fetch(&(sem->cntThreads), 1);

        pthread_t* thr = malloc(sizeof(pthread_t));
        *thr = pthread_self();

        TQueue_Push(sem->thr_queue, thr);

        // release the guard lock
        __sync_val_compare_and_swap(&(sem->guard), true, false);

        // go to sleep
        pause();
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
    sem->guard = false;
    TQueue_Clear(sem->thr_queue);
    return 0;
}