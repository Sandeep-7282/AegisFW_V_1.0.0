/*
 * update_uart.h
 *
 *  Created on: Jan 3, 2026
 *      Author: sande
 */

#ifndef INC_UPDATE_UART_H_
#define INC_UPDATE_UART_H_

#include <stdint.h>
#include <stdbool.h>

bool bl_uart_recv(uint8_t *buf, uint32_t len);
void bl_uart_send_str(const char *s);
bool bl_receive_and_program_image(void);

#endif /* INC_UPDATE_UART_H_ */
