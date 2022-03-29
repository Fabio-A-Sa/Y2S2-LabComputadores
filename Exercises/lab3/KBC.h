#ifndef KBC_H
#define KBC_H

#include <lcom/lcf.h>

/**
 * @brief waits for the next try
 */
void sleep();

/**
 * @brief 
 * @return if the system call is successful or not
 */
int subscribe_KBC_interrupts();

/**
 * @brief 
 * @return if the system call is successful or not
 */
int unsubscribe_KBC_interrupts();


int readFromKBC();
int writeToKBC();

#endif /* KBC_H */