#include "queue.h"

void create_queue(queue_t* q) {
    q->size = 0;
    q->rear = NULL;
    return;
}

void qenqueue(queue_t* q, void* data) {
    queue_node* nn = malloc(sizeof(queue_node));
    nn->data = data;
    if (q->rear == NULL) {
        q->rear = nn;
        nn->next = nn;
        q->size++;
    } else {
        nn->next = q->rear->next;
        q->rear->next = nn;
        q->rear = nn;
        q->size++;
    }
}

void qdequeue(queue_t* q, void** data) {
    if (q->rear == NULL) {
        printf("Can't dequeue from null queue\n'");
        return;
    } else if (q->rear == q->rear->next) {
        *data = q->rear->data;
        free(q->rear);
        q->rear = NULL;
        q->size--;
        return;
    } else {
        *data = q->rear->next->data;
        free(q->rear->next);
        q->rear->next = q->rear->next->next;
        q->size--;
        return;
    }
}

int qexists(queue_t* q, void* data, int (*func)(void* d1, void* d2)) {
    queue_node* curr = q->rear->next;
    do {
        if (func(curr->data, data) == 0) {
            return 1;
        }
    } while((curr = curr->next) != q->rear->next);
    return 0;
}

void qdelete_item(queue_t* q, void* to_delete, void** data, int (*func)(void* d1, void* d2)) {
    queue_node* prev = NULL;
    queue_node* curr = q->rear;
    while((curr = curr->next) != q->rear) {
        if (func(curr->data, to_delete) == 0) {
            if (curr->next == curr) {
                q->rear = NULL;
                *data = curr;
                free(curr);
                q->size--;
                return;
            } else {
                prev->next = curr->next;
                *data = curr;  
                free(curr);
                q->size--;
                return;
            }
        }
        prev = curr;
    }
}