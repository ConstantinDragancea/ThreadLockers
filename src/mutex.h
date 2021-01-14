#ifndef MUTEX_H_
#define MUTEX_H_

#include <stdbool.h>
#include "thread_queue.h"

typedef struct __Mutex Mutex;

struct __Mutex {
    bool is_locked;
    bool sleep_guard;
    bool guard;
    Thread_Queue* thr_queue;
};

// Initializes the mutex
int mutex_init(Mutex* mutex);

// Locks the mutex, ensures only one thread accesses critical section
int mutex_lock(Mutex* mutex);

// Unlocks the mutex, allows others threads to access section
int mutex_unlock(Mutex* mutex);

// Destroys the mutex, frees up the memory
int mutex_destroy(Mutex* mutex);

#endif // MUTEX_H_