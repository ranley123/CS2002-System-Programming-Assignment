#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "queue_r.h"

/**
 * Struct representing a node in a queue_t
 */
typedef struct queue_node_t {
    struct queue_node_t *next;
    void *data;
} queue_node_t;

/**
 * Struct representing a queue
 */
struct queue_t {
    queue_node_t *head, *tail;
    int size;
    int maxSize;
    pthread_cond_t cv;
    pthread_mutex_t m;
};

/**
 *  Given data, place it on the queue.  Can be called by multiple threads.
 *  Blocks if the queue is full.
 */
void enqueue(struct queue_t *queue, void *data) 
{
    pthread_mutex_lock(&(queue->m));

    if(queue->maxSize > 0) {
        while (queue->size >= queue->maxSize) {
            pthread_cond_wait(&(queue->cv), &(queue->m));
        }
    }

    queue_node_t *node = malloc(sizeof(queue_node_t));
    node->next = NULL;
    node->data = data;

    if (queue->size == 0) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }

    queue->size += 1;
    pthread_cond_broadcast(&(queue->cv));
    pthread_mutex_unlock(&(queue->m));
}

/**
 *  Retrieve the data from the front of the queue.  Can be called by multiple
 * threads.
 *  Blocks if the queue is empty.
 */
void * dequeue(struct queue_t *queue) 
{
    pthread_mutex_lock(&(queue->m));

    while (queue->size <= 0) {
        pthread_cond_wait(&(queue->cv), &(queue->m));
    }

    queue_node_t *node;
    node = queue->head;

    if (queue->size == 1) {
        queue->head = NULL;
        queue->tail = NULL;
    } else {
        queue->head = node->next;
    }

    void *data = node->data;
    free(node);

    queue->size -= 1;
    pthread_cond_broadcast(&(queue->cv));
    pthread_mutex_unlock(&(queue->m));

    return data;
}

/**
 *  Allocates heap memory for a queue_t and initializes it.
 *  Returns a pointer to this allocated space.
 */
struct queue_t * queue_create(size_t maxSize) 
{
    struct queue_t *queue = malloc(sizeof(struct queue_t));
    if (!queue)
        return NULL;

    queue->head = NULL;
    queue->tail = NULL;

    queue->size = 0;
    queue->maxSize = maxSize;

    pthread_mutex_init(&(queue->m), NULL);
    pthread_cond_init(&(queue->cv), NULL);

    return queue;
}

/**
 *  Destroys the queue, freeing any remaining nodes in it.
 */
void queue_destroy(struct queue_t *queue, void (*de_structor)(void *element)) 
{
    if (!queue)
        return;

    queue_node_t *next = queue->head;
    queue_node_t *curr;
    while (next) {
        curr = next;
        next = curr->next;
        (*de_structor)(curr->data);
        free(curr);
    }

    pthread_cond_destroy(&(queue->cv));
    pthread_mutex_destroy(&(queue->m));

    free(queue);
}
