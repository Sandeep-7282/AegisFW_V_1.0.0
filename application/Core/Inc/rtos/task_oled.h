/*
 * task_oled.h
 *
 *  Created on: Dec 27, 2025
 *      Author: sande
 */

#ifndef INC_RTOS_TASK_OLED_H_
#define INC_RTOS_TASK_OLED_H_

#include "stdint.h"

typedef struct {
    uint8_t row;
    char text[22];
} oled_msg_t;

void OLED_Task(void *arg);
void OLED_PrintAsync(uint8_t row, const char *s);

#endif /* INC_RTOS_TASK_OLED_H_ */
