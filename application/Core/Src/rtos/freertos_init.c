/*
 * freertos_init.c
 *
 * RTOS object creation and scheduler start for AegisFW.
 *
 * All queues, mutexes, and tasks are created here
 * BEFORE the scheduler starts.
 *
 * Startup sequencing:
 *  - Start_Peripherals_Task initializes late peripherals (TIM, etc.)
 *  - HealthMonitor_Task supervises system liveness
 */

#include "FreeRTOS.h"
#include "task.h"
#include "freertos_init.h"

#include "uart_driver.h"
#include "gpio_driver.h"

#include "tasks_common.h"
#include "health_manager.h"
#include "task_oled.h"
#include "task_sensors.h"
#include "task_cli.h"
#include "task_adc.h"
#include "task_start.h"

/* ---------------- RTOS Objects ---------------- */

QueueHandle_t q_sensors = NULL;
TaskHandle_t  adcTaskHandle = NULL;

/* ---------------- Task Parameters ---------------- */

/* Stack sizes (words, not bytes) */
#define STACK_ADC        256
#define STACK_HEALTH     256
#define STACK_CLI        512
#define STACK_SENSORS    512
#define STACK_OLED       512
#define STACK_START      256

/* Priorities (higher = more critical) */
#define PRIO_HEALTH      5
#define PRIO_CLI         4
#define PRIO_SENSORS     3
#define PRIO_START       3
#define PRIO_ADC         2
#define PRIO_OLED        1

/* ------------------------------------------------ */

void rtos_start(void)
{
    BaseType_t ret;

    /* -------- Create IPC objects -------- */

    q_sensors = xQueueCreate(10, sizeof(sensor_event_t));
    configASSERT(q_sensors != NULL);

    q_oled = xQueueCreate(10, sizeof(oled_msg_t));
    configASSERT(q_oled != NULL);

    uart_mutex = xSemaphoreCreateMutex();
    configASSERT(uart_mutex != NULL);

    uart_send_str("\r\n[RTOS] Creating tasks...\r\n");

    /* -------- Create tasks -------- */

    ret = xTaskCreate(ADC_Task,
                      "ADC",
                      STACK_ADC,
                      NULL,
                      PRIO_ADC,
                      &adcTaskHandle);
    configASSERT(ret == pdPASS);

    ret = xTaskCreate(HealthMonitor_Task,
                      "Health",
                      STACK_HEALTH,
                      NULL,
                      PRIO_HEALTH,
                      NULL);
    configASSERT(ret == pdPASS);

    ret = xTaskCreate(CLI_Task,
                      "CLI",
                      STACK_CLI,
                      NULL,
                      PRIO_CLI,
                      NULL);
    configASSERT(ret == pdPASS);

    ret = xTaskCreate(Sensors_Task,
                      "Sensor",
                      STACK_SENSORS,
                      NULL,
                      PRIO_SENSORS,
                      NULL);
    configASSERT(ret == pdPASS);

    ret = xTaskCreate(OLED_Task,
                      "OLED",
                      STACK_OLED,
                      NULL,
                      PRIO_OLED,
                      NULL);
    configASSERT(ret == pdPASS);

    ret = xTaskCreate(Start_Peripherals_Task,
                      "START",
                      STACK_START,
                      NULL,
                      PRIO_START,
                      NULL);
    configASSERT(ret == pdPASS);

    uart_send_str("[RTOS] Scheduler starting...\r\n");

    /* -------- Start scheduler -------- */

    vTaskStartScheduler();

    /* Scheduler failed to start (heap/config issue) */
    uart_send_str("[RTOS] FATAL: Scheduler start failed\r\n");
    taskDISABLE_INTERRUPTS();

    while (1);
}
