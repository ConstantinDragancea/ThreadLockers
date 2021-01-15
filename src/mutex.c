#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

#include "mutex.h"

void signal_handler(int sig) {
    // printf("Thread %d woke up!\n", pthread_self());
    return;
}

int mutex_init(Mutex* mutex){
    if (mutex == NULL){
        printf("Null mutex given to initialise!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }
    signal(SIGUSR1, signal_handler);
    mutex->is_locked = false;
    mutex->guard = false;
    mutex->thr_queue = malloc(sizeof(Thread_Queue));
    TQueue_Init(mutex->thr_queue);
    return 0;
}

int mutex_lock(Mutex* mutex){
    if (mutex == NULL){
        printf("Null mutex given to lock!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }
    while (1){
        // Spin lock for a bit
        while (1){
            bool value = __sync_val_compare_and_swap(&(mutex->guard), false, true);
            if (value == false){
                // printf("Acquired sleep guard %d\n", pthread_self());
                break;
            }
        }

        bool value = __sync_val_compare_and_swap(&(mutex->is_locked), false, true);
        if (value == false){
            // printf("Mutex locked by thread %d\n", pthread_self());
            __sync_val_compare_and_swap(&(mutex->guard), true, false);
            return 0;
        }
        
        // After spinning for a bit, insert in queue and go to sleep
        pthread_t* thr = malloc(sizeof(pthread_t));
        *thr = pthread_self();

        TQueue_Push(mutex->thr_queue, thr);

        __sync_val_compare_and_swap(&(mutex->guard), true, false);

        // go to sleep
        pause();

    }
    return 0;
}

int mutex_unlock(Mutex* mutex){
    if (mutex == NULL){
        printf("Null mutex given to unlock!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }
    while (1){
        bool value = __sync_val_compare_and_swap(&(mutex->guard), false, true);
        if (value == false){
            break;
        }
    }

    // TQueue_Print(mutex->thr_queue);
    pthread_t* to_wake_up = TQueue_Pop(mutex->thr_queue);
    // printf("thread to wake up %d\n", (to_wake_up == NULL ? -1 : *to_wake_up));
    if (to_wake_up != NULL){

        // doesn't actually kill, just sends the specified signal to the thread
        if (pthread_kill(*to_wake_up, SIGUSR1) != 0){
            printf("Couldn't wake up thread!\n");
            char message[] = "Error message: ";
            perror(message);
        }
        
        // printf("Woke up thread %d\n", *to_wake_up);
    }
    __sync_val_compare_and_swap(&(mutex->is_locked), true, false);

    __sync_val_compare_and_swap(&(mutex->guard), true, false);
    return 0;
}

int mutex_destroy(Mutex* mutex){
    if (mutex == NULL){
        printf("Null mutex given to destroy!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }
    TQueue_Clear(mutex->thr_queue);
    // free(mutex->thr_queue);
    return 0;
}