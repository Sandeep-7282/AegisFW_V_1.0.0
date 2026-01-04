/*
 * task_oled.c
 *
 * OLED display task for AegisFW.
 * Non-critical, failure-aware, watchdog-safe.
 */

#include "task_oled.h"
#include "oled.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "string.h"
#include "health_manager.h"
#include "uart_threadsafe.h"
#include "tasks_common.h"

/* Global queue, created before scheduler */
extern QueueHandle_t q_oled;

void OLED_Task(void *arg)
{
    (void)arg;

    uprintf("[OLED] Task started\r\n");

    /* Initialize display */
    oled_init();

    if (!oled_is_present())
    {
        uprintf("[OLED] Display not detected, task dormant\r\n");

        /* OLED absent → never heartbeat */
        for (;;)
            vTaskDelay(portMAX_DELAY);
    }

    oled_clear();

    oled_msg_t msg;

    for (;;)
    {
        /* Block until work arrives */
        if (xQueueReceive(q_oled, &msg, portMAX_DELAY) == pdTRUE)
        {
            /* Attempt print */
            oled_print(msg.row, msg.text);

            /* Meaningful work succeeded */
            Heartbeat_OLED();
        }
    }
}

/*
 * Safe API: callable from tasks only
 */
void OLED_PrintAsync(uint8_t row, const char *s)
{
    if (!q_oled || !s)
        return;

    oled_msg_t msg;
    msg.row = row;

    strncpy(msg.text, s, sizeof(msg.text) - 1);
    msg.text[sizeof(msg.text) - 1] = '\0';

    /* Non-blocking send */
    xQueueSend(q_oled, &msg, 0);
}
