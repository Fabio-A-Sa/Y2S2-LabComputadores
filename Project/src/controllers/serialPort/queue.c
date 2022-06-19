#include "queue.h"

Queue* createQueue(){
    Queue* queue = (Queue*)malloc(
        sizeof(Queue));
    queue->capacity = QUEUESIZE;
    queue->front = queue->size = 0;
 
    // This is important, see the enqueue
    queue->back = QUEUESIZE - 1;
    queue->values = (int*)malloc(
        queue->capacity * sizeof(int));
    return queue;
}

bool isFull(Queue* queue){
    return (queue->size == (int) queue->capacity);
}

bool isEmpty(Queue* queue){
    return (queue->size == 0);
}

int queueSize(Queue * queue){
    return queue->size;
}

bool push(Queue* queue, uint8_t element){
    if (isFull(queue))
        return false;
    queue->back = (queue->back + 1) % queue->capacity;
    queue->values[queue->back] = element;
    queue->size = queue->size + 1;
    return true;
}

uint8_t pop(Queue* queue){
    if (isEmpty(queue))
        return 0;
    int element = queue->values[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return element;
}

uint8_t front(Queue* queue){
    if (isEmpty(queue))
        return 0;
    return queue->values[queue->front];
}

uint8_t back(Queue* queue){
    if (isEmpty(queue))
        return 0;
    return queue->values[queue->back];
}

void clear(Queue* queue){
    free(queue->values);
    free(queue);
}
