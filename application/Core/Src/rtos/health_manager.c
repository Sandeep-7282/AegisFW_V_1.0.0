/*
 * health_manager.c
 *
 * System health supervision and watchdog control.
 *
 * This task is the ONLY entity allowed to feed the watchdog.
 * All other tasks must report liveness via heartbeats.
 */

#include "health_manager.h"
#include "watchdog.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdbool.h>
#include <stdint.h>

/* ---------------- Heartbeat Bitmask ---------------- */

#define HB_CLI_BIT     (1U << 0)
#define HB_SENSOR_BIT  (1U << 1)
#define HB_ADC_BIT     (1U << 2)
#define HB_OLED_BIT    (1U << 3)

#define HB_ALL_MASK    (HB_CLI_BIT | HB_SENSOR_BIT | \
                        HB_ADC_BIT | HB_OLED_BIT)

/* Updated by tasks, read/cleared by health monitor */
static volatile uint32_t hb_mask;

/* ---------------- Heartbeat APIs ---------------- */

void Heartbeat_CLI(void)    { hb_mask |= HB_CLI_BIT; }
void Heartbeat_Sensor(void) { hb_mask |= HB_SENSOR_BIT; }
void Heartbeat_ADC(void)    { hb_mask |= HB_ADC_BIT; }
void Heartbeat_OLED(void)   { hb_mask |= HB_OLED_BIT; }

/* ---------------- Health Monitor Task ---------------- */

void HealthMonitor_Task(void *arg)
{
    (void)arg;

    const TickType_t period = pdMS_TO_TICKS(5000);
    uint8_t warmup_cycles = 2;   /* Startup grace window */

    while (1)
    {
        vTaskDelay(period);

        if (warmup_cycles)
        {
            warmup_cycles--;
            hb_mask = 0;
            Watchdog_Feed();
            continue;
        }

        uint32_t snapshot = hb_mask;
        hb_mask = 0;   /* Clear for next window */

        if (snapshot == HB_ALL_MASK)
        {
            /* All tasks reported healthy */
            Watchdog_Feed();
        }
        else
        {
            /*
             * One or more tasks failed to report.
             * Do NOT feed watchdog — reset will occur.
             *
             * Fault details can be reconstructed
             * post-reset using reset cause + logs.
             */
        }
    }
}
