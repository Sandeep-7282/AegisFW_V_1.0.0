/*
 * idle_hook.c
 *
 *  Created on: Dec 26, 2025
 *      Author: sande
 */
#include "FreeRTOS.h"
#include "task.h"
#include "uart_driver.h"

static uint32_t idle_counter=0;

void vApplicationIdleHook(void)
{
    idle_counter++;
    if(idle_counter % 50000 == 0)
        uart_send_str("[IDLE] CPU free\r\n");
}


