/**
 * @file queue
 * @brief This file contains the functions for the implements a queue system.
 *
 * @author www.geeksforgeeks.org
 *
 * @date 25/05/2022
 */

#ifndef __QUEUE_H_
#define __QUEUE_H_

#define QUEUESIZE 100

#include <lcom/lcf.h>


typedef struct {
    int front, back, size;
    unsigned capacity;
    int* values;
}Queue;

Queue* createQueue();
int queueSize(Queue *queue);
bool push(Queue* queue, uint8_t element);
uint8_t pop(Queue* queue);
uint8_t front(Queue* queue);
uint8_t back(Queue* queue);
bool isEmpty(Queue* queue);
bool isFull(Queue* queue);
void clear(Queue* queue);

#endif
