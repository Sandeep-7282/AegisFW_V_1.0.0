/*
 * task_start.c
 *
 * Deferred peripheral initialization task for AegisFW.
 * Runs once, confirms startup, then self-deletes.
 */

#include "timer2.h"
#include "uart_threadsafe.h"
#include "FreeRTOS.h"
#include "task.h"
#include "health_manager.h"

void Start_Peripherals_Task(void *arg)
{
    (void)arg;

    /* Allow RTOS, queues, and tasks to stabilize */
    vTaskDelay(pdMS_TO_TICKS(500));

    uprintf("[START] Initializing deferred peripherals...\r\n");

    /* Start periodic timer */
    tim2_init(10);   /* 10 Hz sensor tick */

    uprintf("[START] TIM2 started\r\n");

    /*
     * Signal successful completion once.
     * This confirms the system reached full operational state.
     */
    Heartbeat_Sensor();

    /* Task has fulfilled its purpose */
    uprintf("[START] Initialization complete, task exiting\r\n");

    vTaskDelete(NULL);
}
