#include "rwlock.h"

int rwlock_init(rwlock* rwl){
    if (rwl == NULL){
        printf("Null read-write lock given to init!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }
    rwl->readers = 0;
    mutex_init(&(rwl->intent));
    mutex_init(&(rwl->empty));
    mutex_init(&(rwl->mtx));
    return 0;
}

int rwlock_destroy(rwlock* rwl){
    if (rwl == NULL){
        printf("Null read-write lock given to destroy!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }
    rwl->readers = 0;
    mutex_destroy(&(rwl->intent));
    mutex_destroy(&(rwl->empty));
    mutex_destroy(&(rwl->mtx));
    return 0;
}

int rwlock_read_lock(rwlock* rwl){
    if (rwl == NULL){
        printf("Null read-write lock given to read-lock!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }
    mutex_lock(&(rwl->intent));
    mutex_lock(&(rwl->mtx));
    rwl->readers++;
    if (rwl->readers == 1){
        mutex_lock(&(rwl->empty));
    }
    mutex_unlock(&(rwl->mtx));
    mutex_unlock(&(rwl->intent));
    return 0;
}

int rwlock_read_unlock(rwlock* rwl){
    if (rwl == NULL){
        printf("Null read-write lock given to read-unlock!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }
    mutex_lock(&(rwl->mtx));
    rwl->readers--;
    if (rwl->readers == 0){
        mutex_unlock(&(rwl->empty));
    }
    mutex_unlock(&(rwl->mtx));
    return 0;
}

int rwlock_write_lock(rwlock* rwl){
    if (rwl == NULL){
        printf("Null read-write lock given to write-lock!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }
    mutex_lock(&(rwl->intent));
    mutex_lock(&(rwl->empty));
    return 0;
}

int rwlock_write_unlock(rwlock* rwl){
    if (rwl == NULL){
        printf("Null read-write lock given to write-unlock!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }
    mutex_unlock(&(rwl->empty));
    mutex_unlock(&(rwl->intent));
    return 0;
}