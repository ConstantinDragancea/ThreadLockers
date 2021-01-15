#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <stdbool.h>
#include "thread_queue.h"

typedef struct Semaphore Semaphore;

struct Semaphore {
    int cntThreads;
    bool guard;
    Thread_Queue* thr_queue;
};

// Initialize the semaphore
int semaphore_init(Semaphore* sem, int intialValue);

// Increments the semaphore, allows other threads to get the lock
int semaphore_post(Semaphore* sem);

// Decrements the semaphore; the call blocks until the semaphore value is greater than zero
int semaphore_wait(Semaphore* sem);

// Destroy the semaphore
int semaphore_destroy(Semaphore* sem);

#endif // SEMAPHORE_H_