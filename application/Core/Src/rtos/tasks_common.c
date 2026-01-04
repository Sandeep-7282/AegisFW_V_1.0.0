/*
 * tasks_common.c
 *
 * Global RTOS synchronization objects and health flags for AegisFW.
 *
 * Ownership rules:
 *  - Heartbeat flags are SET by individual tasks
 *  - Heartbeat flags are CLEARED by HealthMonitor_Task
 *  - No task clears its own heartbeat
 *
 * All objects here are initialized before scheduler start.
 */

#include "tasks_common.h"

/* ---------------- RTOS objects ---------------- */

/* OLED message queue (created in freertos_init.c) */
QueueHandle_t q_oled = NULL;

/* UART mutex for threadsafe logging */
SemaphoreHandle_t uart_mutex = NULL;

/* ---------------- Health monitoring ---------------- */

/*
 * Heartbeat flags:
 *  - Set to 1 by task when it successfully runs
 *  - Cleared periodically by HealthMonitor_Task
 *  - If not set within health window → fault
 */
volatile uint8_t hb_cli    = 0;
volatile uint8_t hb_sensor = 0;
volatile uint8_t hb_adc    = 0;
volatile uint8_t hb_oled   = 0;
