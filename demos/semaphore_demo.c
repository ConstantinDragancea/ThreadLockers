#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#include "mutex.h"
#include "semaphore.h"

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