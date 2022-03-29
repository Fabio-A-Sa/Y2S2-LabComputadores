#ifndef KBC_H
#define KBC_H

/**
 * @brief waits for the next try
 */
void (sleep)();

/**
 * @brief subscribe KCB interrupts
 * @return if the system call is successful or not
 */
int (subscribe_KBC_interrupts)(uint8_t *bit_no);

/**
 * @brief unsubscribe KCB interrups
 * @return if the system call is successful or not
 */
int (unsubscribe_KBC_interrupts)();

/**
 * @brief 
 * 
 * @return if the system call is successful or not
 */
int (readFromKBC)();

/**
 * @brief handles keyboard interrupts
 */
void (kbc_in)();

#endif /* KBC_H */