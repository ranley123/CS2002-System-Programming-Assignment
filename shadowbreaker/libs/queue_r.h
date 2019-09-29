#pragma once /* This is a non-portable equivalent of #ifndef header guard.*/

#include <stdlib.h>


/**
 * Struct representing a queue
 * This 'forward hides' struct queue_t from others.
 */
typedef struct queue_t queue;

/**
 * Note: Functions such as queue_empty(), queue_size(), and queue_peek() 
 * cannot be provided in a thread-safe manner. Why?
 * Because they are 'state' values that are owned by the queue 
 * and can not be passed off to any individual thread. In other words,
 * the returned values could will be stale, even incorrect, by the 
 * time they are returned.
 * Therefore it is impossible to expose them in a thread-safe manner.
 *
 * Consider the following timeline of two threads with a brand new queue:
 *
 *   time |            thread 1            |         thread 2
 * --------------------------------------------------------------------
 *    0  | size_t size = queue_size(queue);|
 *    1  |                                 |  enqueue(queue, blah);
 *    2  | if(size == 0) {do_something()}; |
 *
 * Notice that do_something() will execute when the queue is size 1 (not 0).
 * This is a race condition that emerges even if queue_size() is implemented
 * in a thread-safe manner!
 * 
 * Similar arguments can be made for queue_empty() and queue_peek().
 */

/**
 * Allocate and return a pointer to a new queue (on the heap).
 *
 * 'max_size' determines how many elements the user can add to the queue before
 * it blocks.
 * If non-positive, the queue will never block upon a push (the queue does not
 * have a 'max_size').
 * Else, the queue will block if the user tries to push when the queue is full.
 */
queue *queue_create(size_t max_size);

/**
 * Adds a new 'element' to the end of the queue in constant time.
 * Note: Can be called by multiple threads.
 * Note: Blocks if the queue is full (defined by it's max_size).
 */
void enqueue(queue *q, void *element);

/**
 * Removes the element at the front of the queue in constant time.
 * Note: Can be called by multiple threads.
 * Note: Blocks if the queue is empty.
 */
void *dequeue(queue *q);

/**
 * Destroys all queue elements by deallocating all the storage capacity
 * allocated by the 'queue'.
 * Also calls free(q) -- best to then "q = NULL;" after calling this function!
 * 
 * The second argument is a ptr to a function that must do the following:
 *   - takes a pointer to element that will be free'd
 *   - casts that element pointer appropriately (eg. to a 'task')
 *   - frees the element appropriately
 *   - returns void.
 */
void queue_destroy(queue *q, void (*de_structor)(void *element));

