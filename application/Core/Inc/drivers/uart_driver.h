/*
 * uart_driver.h
 *
 *  Created on: Dec 9, 2025
 *      Author: sande
 */

#ifndef INC_UART_DRIVER_H_
#define INC_UART_DRIVER_H_


#include "stm32f446xx.h"
#include <stdint.h>

void uart_init(uint32_t baud);

void uart_send_byte(uint8_t byte);
void uart_send_str(const char *s);

int  uart_rx_available(void);
char uart_read(void);        // blocking read of one byte
char uart_read_nonblock(void); // optional non-blocking read

uint8_t uart_get_overflow_flag(void);
void    uart_clear_overflow_flag(void);

#endif /* INC_UART_DRIVER_H_ */
