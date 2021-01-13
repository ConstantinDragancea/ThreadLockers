#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#include "mutex.h"
#include "semaphore.h"

// Simulare de lab 7 ex 1
/* #define MAX_RESOURCES 15
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
} */

// Simulare de lab 7 ex 2

#define NTHRS 5
int pos[NTHRS];
int barrierCnt;

Mutex mtx;
Semaphore sem;

void init(int nr) {
    barrierCnt = nr;
    if (semaphore_init(&sem, 0)) {
        printf("Can't init sem\n");
        printf("Errno: %d\n", errno);
        return;
    }
}

void barrier_point() {
    mutex_lock(&mtx);

    barrierCnt--;

    if (barrierCnt == 0) {
        mutex_unlock(&mtx);
        if (semaphore_post(&sem)) {
            printf("Can't post sem\n");
            printf("Errno: %d\n", errno);
            return;
        }
        return;
    }
    
    mutex_unlock(&mtx);

    if (semaphore_wait(&sem)) {
        printf("Can't wait sem\n");
        printf("Errno: %d\n", errno);
        return;
    }

    if (semaphore_post(&sem)) {
        printf("Can't post sem\n");
        printf("Errno: %d\n", errno);
        return;
    }
    return;
}

void* tfun(void* v) {
    int *tid = (int*)v;
    printf("%d reached the barrier\n", *tid);
    barrier_point();
    printf("%d passed the barrier\n", *tid);
    
    return NULL;
}

int main() {

    if (mutex_init(&mtx)) {
        printf("Can't init mtx\n");
        return 0;
    }

    printf("NTHRS = %d\n", NTHRS);

    init(NTHRS);
    pthread_t threads[NTHRS];
    for (int i = 0; i < NTHRS; ++i) {
        pos[i] = i;
        if (pthread_create(&threads[i], NULL, tfun, (void*) &pos[i])) {
            printf("Can't create thread\n");
            printf("Errno: %d\n", errno);
            return 0;
        }
    }

    for (int i = 0; i < NTHRS; ++i)
        if (pthread_join(threads[i], NULL)) {
            printf("Can't join thread\n");
            printf("Errno: %d\n", errno);
            return 0;
        }

    mutex_destroy(&mtx);
    semaphore_destroy(&sem);
    return 0;
}