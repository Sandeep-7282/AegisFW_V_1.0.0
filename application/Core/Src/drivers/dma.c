/*
 * dma.c
 *
 *  DMA driver for ADC1 (circular mode).
 *
 */

#include "dma.h"
#include "stm32f446xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "task_adc.h"

/* ADC DMA circular buffer */
volatile uint16_t adc_dma_buffer[ADC_DMA_BUF_LEN];

/* Notify ADC task every N DMA cycles */
#define ADC_DMA_NOTIFY_PERIOD   100U

void dma2_adc1_init(void)
{
    /* Enable DMA2 clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

    /* Disable stream before configuration */
    DMA2_Stream0->CR &= ~DMA_SxCR_EN;
    while (DMA2_Stream0->CR & DMA_SxCR_EN);

    /* Peripheral address: ADC1 data register */
    DMA2_Stream0->PAR  = (uint32_t)&ADC1->DR;

    /* Memory address */
    DMA2_Stream0->M0AR = (uint32_t)adc_dma_buffer;

    /* Number of transfers */
    DMA2_Stream0->NDTR = ADC_DMA_BUF_LEN;

    /* Clear all pending interrupt flags */
    DMA2->LIFCR =
        DMA_LIFCR_CTCIF0 |
        DMA_LIFCR_CHTIF0 |
        DMA_LIFCR_CTEIF0 |
        DMA_LIFCR_CDMEIF0 |
        DMA_LIFCR_CFEIF0;

    /* Configure DMA stream */
    DMA2_Stream0->CR =
          (0U << DMA_SxCR_CHSEL_Pos) |   /* Channel 0 */
          DMA_SxCR_PL_1              |   /* High priority */
          DMA_SxCR_MSIZE_0           |   /* Memory = 16-bit */
          DMA_SxCR_PSIZE_0           |   /* Peripheral = 16-bit */
          DMA_SxCR_MINC              |   /* Memory increment */
          DMA_SxCR_CIRC              |   /* Circular mode */
          DMA_SxCR_TCIE;                 /* Transfer complete IRQ */

    /* Peripheral-to-memory */
    DMA2_Stream0->CR &= ~DMA_SxCR_DIR;

    /* Enable DMA stream */
    DMA2_Stream0->CR |= DMA_SxCR_EN;

    /* Configure NVIC */
    NVIC_SetPriority(DMA2_Stream0_IRQn, 7);
    NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

void DMA2_Stream0_IRQHandler(void)
{
    static uint32_t tc_count = 0;
    BaseType_t higher_prio_woken = pdFALSE;

    /* Transfer complete interrupt */
    if (DMA2->LISR & DMA_LISR_TCIF0)
    {
        /* Clear TC flag first */
        DMA2->LIFCR = DMA_LIFCR_CTCIF0;

        tc_count++;

        if (tc_count >= ADC_DMA_NOTIFY_PERIOD)
        {
            tc_count = 0;

            if (adcTaskHandle != NULL)
            {
                vTaskNotifyGiveFromISR(adcTaskHandle,
                                       &higher_prio_woken);
            }
        }
    }

    portYIELD_FROM_ISR(higher_prio_woken);
}
