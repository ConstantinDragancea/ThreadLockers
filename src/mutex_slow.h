#ifndef MUTEX_SLOW_H_
#define MUTEX_SLOW_H_

#include <stdbool.h>

typedef struct __Mutex Mutex;

struct __Mutex {
    bool is_locked;
    int lol;
};

// Initializes the mutex
int mutex_init(Mutex* mutex);

// Locks the mutex, ensures only one thread accesses critical section
int mutex_lock(Mutex* mutex);

// Unlocks the mutex, allows others threads to access section
int mutex_unlock(Mutex* mutex);

// Destroys the mutex, frees up the memory
int mutex_destroy(Mutex* mutex);

#endif // MUTEX_SLOW_H_ 