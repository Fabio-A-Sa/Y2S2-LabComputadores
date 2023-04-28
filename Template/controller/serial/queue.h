#ifndef _LCOM_QUEUE_H_
#define _LCOM_QUEUE_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

#define QUEUE_SIZE 100

// Data structure
typedef struct {
    uint8_t front, back, size;
    uint8_t capacity;
    uint8_t* values;
} Queue;

// Functions
Queue* create_queue();
int destroy_queue(Queue *queue);
int is_empty(Queue* queue);
int is_full(Queue* queue);
int queue_size(Queue *queue, uint8_t *size);
int pop(Queue* queue, uint8_t *value);
int front(Queue* queue, uint8_t *value);
int back(Queue* queue, uint8_t *value);
int push(Queue* queue, uint8_t element);
void clear(Queue* queue);

#endif
