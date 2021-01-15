#include "rwlock.h"

int rwlock_init(rwlock* rwl){
    rwl->readers = 0;
    mutex_init(&(rwl->intent));
    mutex_init(&(rwl->empty));
    mutex_init(&(rwl->mtx));
}

int rwlock_destroy(rwlock* rwl){
    rwl->readers = 0;
    mutex_destroy(&(rwl->intent));
    mutex_destroy(&(rwl->empty));
    mutex_destroy(&(rwl->mtx));
}

int rwlock_read_lock(rwlock* rwl){
    mutex_lock(&(rwl->intent));
    mutex_lock(&(rwl->mtx));
    rwl->readers++;
    if (rwl->readers == 1){
        mutex_lock(&(rwl->empty));
    }
    mutex_unlock(&(rwl->mtx));
    mutex_unlock(&(rwl->intent));
}

int rwlock_read_unlock(rwlock* rwl){
    mutex_lock(&(rwl->mtx));
    rwl->readers--;
    if (rwl->readers == 0){
        mutex_unlock(&(rwl->empty));
    }
    mutex_unlock(&(rwl->mtx));
}

int rwlock_write_lock(rwlock* rwl){
    mutex_lock(&(rwl->intent));
    mutex_lock(&(rwl->empty));
}

int rwlock_write_unlock(rwlock* rwl){
    mutex_unlock(&(rwl->empty));
    mutex_unlock(&(rwl->intent));
}