#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "mutex.h"

int mutex_init(Mutex* mutex){
    if (mutex == NULL){
        printf("Null mutex given to initialise!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }
    mutex->is_locked = false;
    return 0;
}

int mutex_lock(Mutex* mutex){
    if (mutex == NULL){
        printf("Null mutex given to lock!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }
    while(1){
        bool value = __sync_val_compare_and_swap(&(mutex->is_locked), false, true);
        if (value == false)
            break;
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
    bool res = __sync_val_compare_and_swap(&(mutex->is_locked), true, false);
    res = res;
    return 0;
}

int mutex_destroy(Mutex* mutex){
    if (mutex == NULL){
        printf("Null mutex given to destroy!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }
    return 0;
}

