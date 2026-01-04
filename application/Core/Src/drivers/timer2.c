/*
 * tim2.c
 *
 *  TIM2 periodic interrupt driver for AegisFW.
 *  Used as a low-frequency event source for sensor tasks.
 */

#include "stm32f446xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timer2.h"
#include "task_sensors.h"

void tim2_init(uint32_t freq_hz)
{
    if (freq_hz == 0)
        return;   /* invalid configuration */

    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    /*
     * APB1 timer clock = 84 MHz
     * Prescaler: 84 MHz / 42000 = 2 kHz
     * ARR computed to achieve requested frequency
     */
    uint32_t prescaler = 42000 - 1;
    uint32_t arr       = (2000 / freq_hz) - 1;

    TIM2->PSC  = prescaler;
    TIM2->ARR  = arr;
    TIM2->CNT  = 0;

    TIM2->DIER |= TIM_DIER_UIE;      /* Update interrupt enable */

    NVIC_SetPriority(TIM2_IRQn, 5);
    NVIC_EnableIRQ(TIM2_IRQn);

    TIM2->CR1 |= TIM_CR1_CEN;        /* Start timer */
}

void TIM2_IRQHandler(void)
{
    if (TIM2->SR & TIM_SR_UIF)
    {
        TIM2->SR &= ~TIM_SR_UIF;     /* Clear update flag */

        sensor_event_t ev = SENSOR_TIMER_TICK;
        BaseType_t hp = pdFALSE;

        xQueueSendFromISR(q_sensors, &ev, &hp);
        portYIELD_FROM_ISR(hp);
    }
}
