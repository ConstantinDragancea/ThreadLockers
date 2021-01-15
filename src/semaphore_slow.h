#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

typedef struct Semaphore_Slow Semaphore_Slow;

struct Semaphore_Slow {
    int cntThreads;
};

// Initialize the semaphore
int semaphore_slow_init(Semaphore_Slow* sem, int intialValue);

// Increments the semaphore, allows other threads to get the lock
int semaphore_slow_post(Semaphore_Slow* sem);

// Decrements the semaphore; the call blocks until the semaphore value is greater than zero
int semaphore_slow_wait(Semaphore_Slow* sem);

// Destroy the semaphore
int semaphore_slow_destroy(Semaphore_Slow* sem);

#endif // SEMAPHORE_H_