#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#include "mutex.h"
#include "semaphore.h"

// Simulare de lab 7 ex 1
#define MAX_RESOURCES 15
#define MAX_PROCESSES 10
int available_resources = MAX_RESOURCES;
Mutex mtx;

int decrease_count(int count){
    mutex_lock(&mtx);
    if (available_resources < count){
        mutex_unlock(&mtx);
        return -1;
    }
    available_resources -= count;
    printf("Got %d resources %d remaining\n", count, available_resources);
    mutex_unlock(&mtx);
    return 0;
}

int increase_count(int count){
    mutex_lock(&mtx);
    available_resources += count;
    printf("Released %d resources %d remaining\n", count, available_resources);
    mutex_unlock(&mtx);
    return 0;
}

void* fun(void *v){
    int* val = (int *) v;
    if (decrease_count(*val) == -1){
        free(val);
        return NULL;
    }
    sleep(1);
    increase_count(*val);
    free(val);
    return NULL;
}

int main(){
    // Mutex mtx;
    // mutex_init(&mtx);
    // if (mutex_unlock(&mtx)){
    //     perror("hz brat");
    //     return errno;
    // }
    // mutex_lock(&mtx);
    // mutex_destroy(&mtx);
    // printf("Hello world!\n");

    // Simulare de lab 7 ex 1

    if (mutex_init(&mtx)){
        perror(NULL);
        return errno;
    }

    printf("Max resources: %d\n", MAX_RESOURCES);
        
    pthread_t thr[MAX_PROCESSES];
    srand(time);

    for (int i = 0; i < MAX_PROCESSES; i++){
        int* aux = (int*) malloc(sizeof(int));
        (*aux) = (rand() % 3) + 1;
        if (pthread_create(&thr[i], NULL, fun, aux)){
            perror(NULL);
            return errno;
        }
    }

    for (int i = 0; i < MAX_PROCESSES; i++){
        if (pthread_join(thr[i], NULL)){
            perror(NULL);
            return errno;
        }
    }

    mutex_destroy(&mtx);

    return 0;
}