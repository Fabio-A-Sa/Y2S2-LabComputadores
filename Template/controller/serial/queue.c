#include "queue.h"

Queue* create_queue() {
    Queue* queue = (Queue*) malloc(sizeof(Queue));
    queue->capacity = QUEUE_SIZE;
    queue->front = 0;
    queue->size = 0;
    queue->back = QUEUE_SIZE - 1;
    queue->values = (uint8_t*) malloc(queue->capacity * sizeof(uint8_t));
    return queue;
}

int destroy_queue(Queue *queue) {
    free(queue->values);
    free(queue);
    return 0;
}

int is_full(Queue *queue) {
    return queue->size == queue->capacity;
}

int is_empty(Queue *queue) {
    return queue->size == 0;
}

int queue_size(Queue *queue, uint8_t *size) {
    *size = queue->size;
    return 0;
}

int push(Queue *queue, uint8_t element) {
    if (is_full(queue)) return 1;
    queue->back = (queue->back + 1) % queue->capacity;
    queue->values[queue->back] = element;
    queue->size = queue->size + 1;
    return 0;
}

int pop(Queue *queue, uint8_t *value) {
    if (is_empty(queue)) return 1;
    *value = queue->values[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return 0;
}

int front(Queue *queue, uint8_t *value){
    if (is_empty(queue)) return 1;
    *value = queue->values[queue->front];
    return 0;
}

int back(Queue *queue, uint8_t *value){
    if (is_empty(queue)) return 1;
    *value = queue->values[queue->back];
    return 0;
}

void clear(Queue* queue){
    free(queue->values);
    free(queue);
}
