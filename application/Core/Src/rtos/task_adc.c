/*
 * task_adc.c
 *
 * ADC processing task.
 * Receives notifications from DMA ISR.
 */

#include "task_adc.h"
#include "tasks_common.h"
#include "uart_threadsafe.h"
#include "adc.h"
#include "dma.h"
#include "FreeRTOS.h"
#include "task.h"
#include "health_manager.h"

/* Provided by dma.c */
extern volatile uint16_t adc_dma_buffer[ADC_DMA_BUF_LEN];

void ADC_Task(void *arg)
{
    (void)arg;

    adcTaskHandle = xTaskGetCurrentTaskHandle();

    /* Allow ADC + DMA to stabilize */
    vTaskDelay(pdMS_TO_TICKS(200));

    for (;;)
    {
        /* Wait for DMA notification */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        /* Copy DMA buffer locally (ownership boundary) */
        uint16_t temp_raw = adc_dma_buffer[0];
        uint16_t vref_raw = adc_dma_buffer[1];

        /* Compute values */
        float vref = adc_compute_vref(vref_raw);
        float temp = adc_compute_temperature(temp_raw, vref);

        /* Report liveness */
        Heartbeat_ADC();

#if defined(ADC_DEBUG)
        /* Debug-only logging (disable in production) */
        uprintf("[ADC] Temp=%.2fC  rawT=%u rawV=%u\r\n",
                temp, temp_raw, vref_raw);
#endif
    }
}
