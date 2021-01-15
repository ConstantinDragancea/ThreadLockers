#ifndef MUTEX_SLOW_H_
#define MUTEX_SLOW_H_

#include <stdbool.h>

typedef struct __Mutex_Slow Mutex_Slow;

struct __Mutex_Slow {
    bool is_locked;
};

// Initializes the mutex
int mutex_slow_init(Mutex_Slow* mutex);

// Locks the mutex, ensures only one thread accesses critical section
int mutex_slow_lock(Mutex_Slow* mutex);

// Unlocks the mutex, allows others threads to access section
int mutex_slow_unlock(Mutex_Slow* mutex);

// Destroys the mutex, frees up the memory
int mutex_slow_destroy(Mutex_Slow* mutex);

#endif // MUTEX_SLOW_H_ 