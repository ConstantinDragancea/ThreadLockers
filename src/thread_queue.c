#include "thread_queue.h"

void Free_Node(Node* node){
    free(node->thread);
    free(node);
}

void TQueue_Init(Thread_Queue* tq){
    // TQueue_Clear(tq);
    tq = malloc(sizeof(Thread_Queue));
    tq->count = 0;
    tq->front = NULL;
    tq->back = NULL;
    // printf("Initialised the queue!\n");
}

void TQueue_Push(Thread_Queue* tq, pthread_t* thr){
    if (tq == NULL){
        printf("Tried to push to uninitialised queue!\n");
        exit(-1);
    }

    Node* node = malloc(sizeof(Node));
    node->thread = thr;
    node->next = NULL;

    if (tq->front == NULL){
        tq->front = node;
        tq->back = node;
        tq->count = 1;
    }
    else{
        tq->back->next = node;
        tq->back = node;
        tq->count++;
    }
}

pthread_t* TQueue_Front(Thread_Queue* tq){
    if (tq == NULL){
        printf("Tried to access uninitialised queue!\n");
        exit(-1);
    }
    return tq->front;
}

pthread_t* TQueue_Pop(Thread_Queue* tq){
    if (tq == NULL){
        printf("Tried to pop from uninitialised queue!\n");
        exit(-1);
    }
    if (tq->front == NULL){
        return NULL;
    }
    Node* result = tq->front;
    tq->front = tq->front->next;
    tq->count--;
    if (tq->count == 0){
        tq->back = NULL;
    }

    return result->thread;
}

void TQueue_Clear(Thread_Queue* tq){
    if (tq == NULL){
        return;
    }
    Node* current;
    Node* next;
    for (current = tq->front; current != NULL; current = next){
        next = current->next;
        Free_Node(current);
    }
    free(tq);
}

void TQueue_Print(Thread_Queue* tq){
    if (tq->front == NULL){
        printf("Empty queue --------------------------\n");
        return;
    }
    for (Node* node = tq->front; node; node = node->next){
        printf("%d --> ", *(node->thread));
    }
    printf("\n");
}