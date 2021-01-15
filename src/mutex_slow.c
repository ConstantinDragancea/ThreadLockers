#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "mutex_slow.h"

int mutex_slow_init(Mutex_Slow* mutex){
    if (mutex == NULL){
        printf("Null mutex given to initialise!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }
    mutex->is_locked = false;
    return 0;
}

int mutex_slow_lock(Mutex_Slow* mutex){
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

int mutex_slow_unlock(Mutex_Slow* mutex){
    if (mutex == NULL){
        printf("Null mutex given to unlock!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }
    bool res = __sync_val_compare_and_swap(&(mutex->is_locked), true, false);
    return 0;
}

int mutex_slow_destroy(Mutex_Slow* mutex){
    if (mutex == NULL){
        printf("Null mutex given to destroy!\n");
        char message[] = "Error message: ";
        perror(message);
        return -1;
    }
    return 0;
}