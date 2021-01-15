#ifndef RWLOCKS_H_
#define RWLOCKS_H_

#include <stdbool.h>
#include "thread_queue.h"
#include "mutex.h"

typedef struct rwlock rwlock;

struct rwlock {
    int readers;
    Mutex intent;
    Mutex empty;
    Mutex mtx;
};

int rwlock_init(rwlock* rwl);

int rwlock_destroy(rwlock* rwl);

int rwlock_read_lock(rwlock* rwl);

int rwlock_read_unlock(rwlock* rwl);

int rwlock_write_lock(rwlock* rwl);

int rwlock_write_unlock(rwlock* rwl);

#endif // RWLOCKS_H_