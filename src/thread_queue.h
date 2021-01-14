#ifndef THREAD_QUEUE_H_
#define THREAD_QUEUE_H_

#include <pthread.h>

typedef struct __Node Node;

struct __Node {
    pthread_t* thread;
    Node* next;
};

typedef struct Thread_Queue{
    int count;
    Node* front;
    Node* back;
} Thread_Queue;

void Free_Node(Node* node);

void TQueue_Init(Thread_Queue* tq);

void TQueue_Push(Thread_Queue* tq, pthread_t* thr);

pthread_t* TQueue_Front(Thread_Queue* tq);

pthread_t* TQueue_Pop(Thread_Queue* tq);

void TQueue_Clear(Thread_Queue* tq);

void TQueue_Print(Thread_Queue* tq);

#endif // THREAD_QUEUE_H_