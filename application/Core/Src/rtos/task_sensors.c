/*
 * task_sensors.c
 *
 * Sensor event task and EXTI ISR dispatcher.
 */

#include "task_sensors.h"
#include "uart_threadsafe.h"
#include "tasks_common.h"
#include "freertos_init.h"
#include "health_manager.h"
#include "FreeRTOS.h"
#include "task.h"

/* Debounce only where needed (impact sensor) */
static volatile TickType_t lastImpactTick = 0;
#define IMPACT_DEBOUNCE_MS   50

void Sensors_Task(void *arg)
{
    (void)arg;

    uprintf("[SENSORS] Task started\r\n");

    sensor_event_t ev;

    for (;;)
    {
        /* Block until an event arrives */
        if (xQueueReceive(q_sensors, &ev, portMAX_DELAY) == pdTRUE)
        {
            /* Task successfully handled work → heartbeat */
            Heartbeat_Sensor();

            switch (ev)
            {
                case SENSOR_PIR:
                    uprintf("[SENS] PIR triggered\r\n");
                    break;

                case SENSOR_IMPACT:
                    uprintf("[SENS] Impact detected\r\n");
                    break;

                case SENSOR_BUTTON:
                    uprintf("[SENS] Button pressed\r\n");
                    break;

                case SENSOR_TIMER_TICK:
                    uprintf("[SENS] Timer tick\r\n");
                    break;

                default:
                    break;
            }
        }
    }
}

/*
 * Called from EXTI IRQ context
 */
void Sensors_ISR_Handler(uint8_t pin)
{
    BaseType_t hp = pdFALSE;
    sensor_event_t ev = SENSOR_NONE;

    switch (pin)
    {
        case 0:
            ev = SENSOR_PIR;
            break;

        case 5:   /* Impact sensor */
        {
            TickType_t now = xTaskGetTickCountFromISR();
            if ((now - lastImpactTick) >= pdMS_TO_TICKS(IMPACT_DEBOUNCE_MS))
            {
                lastImpactTick = now;
                ev = SENSOR_IMPACT;
            }
            break;
        }

        case 13:
            ev = SENSOR_BUTTON;
            break;

        default:
            break;
    }

    if (ev != SENSOR_NONE)
    {
        xQueueSendFromISR(q_sensors, &ev, &hp);
        portYIELD_FROM_ISR(hp);
    }
}
