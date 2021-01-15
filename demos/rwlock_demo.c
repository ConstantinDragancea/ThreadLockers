#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#include "rwlock.h"

#define MAX_PROCESSES 15
#define WRITER_FREQ 10

rwlock rwl;
int book;

void* writer_fun(void *v){
    rwlock_write_lock(&rwl);
    sleep(1);
    book++;
    rwlock_write_unlock(&rwl);
    return NULL;
}

void* reader_fun(void *v){
    rwlock_read_lock(&rwl);
    printf("Book: %d\n", book);
    sleep(1);
    rwlock_read_unlock(&rwl);
    return NULL;
}

int main(){
    rwlock_init(&rwl);

    printf("Max resources: %d\n", MAX_PROCESSES);
        
    pthread_t thr[MAX_PROCESSES + 1];
    srand(time);

    for (int i = 1; i <= MAX_PROCESSES; i++){
        if (rand() % 3 == 0){
            printf("Tatal porneste threadul %d writer\n", i);
            // writer thread
            if (pthread_create(&thr[i], NULL, writer_fun, NULL)){
                perror(NULL);
                return errno;
            }
        }
        else {
            printf("Tatal porneste threadul %d reader\n", i);
            if (pthread_create(&thr[i], NULL, reader_fun, NULL)){
                perror(NULL);
                return errno;
            }
        }
    }

    for (int i = 1; i <= MAX_PROCESSES; i++){
        if (pthread_join(thr[i], NULL)){
            perror(NULL);
            return errno;
        }
    }

    rwlock_destroy(&rwl);
    return 0;
}