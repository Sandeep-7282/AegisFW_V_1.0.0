/*
 * gpio_exti.c
 *
 *  External interrupt handling for sensor inputs.
 *
 *  ISR responsibility:
 *  - Clear EXTI pending bit
 *  - Signal sensor task
 *  - Exit immediately
 *
 */

#include "gpio_exti.h"
#include "stm32f446xx.h"
#include "task_sensors.h"
#include "FreeRTOS.h"
#include "task.h"

/* Enable GPIO clocks */
#define GPIO_ENABLE_PORTA()  (RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN)
#define GPIO_ENABLE_PORTB()  (RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN)
#define GPIO_ENABLE_PORTC()  (RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN)

void GPIO_EXTI_Init(void)
{
    GPIO_ENABLE_PORTA();
    GPIO_ENABLE_PORTB();
    GPIO_ENABLE_PORTC();

    /* ---- GPIO INPUT CONFIG ---- */

    /* PIR: PA0 */
    GPIOA->MODER &= ~(3U << (0 * 2));
    GPIOA->PUPDR &= ~(3U << (0 * 2));
    GPIOA->PUPDR |=  (1U << (0 * 2));   /* Pull-up */

    /* Impact sensor: PB5 */
    GPIOB->MODER &= ~(3U << (5 * 2));
    GPIOB->PUPDR &= ~(3U << (5 * 2));
    GPIOB->PUPDR |=  (1U << (5 * 2));

    /* User button: PC13 */
    GPIOC->MODER &= ~(3U << (13 * 2));
    GPIOC->PUPDR &= ~(3U << (13 * 2));
    GPIOC->PUPDR |=  (1U << (13 * 2));

    /* ---- EXTI MUX ---- */
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;          /* PA0 */
    SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI5;
    SYSCFG->EXTICR[1] |=  SYSCFG_EXTICR2_EXTI5_PB;       /* PB5 */
    SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
    SYSCFG->EXTICR[3] |=  SYSCFG_EXTICR4_EXTI13_PC;      /* PC13 */

    /* ---- EXTI TRIGGERS ---- */
    EXTI->IMR  |= (1U << 0) | (1U << 5) | (1U << 13);
    EXTI->RTSR |= (1U << 0) | (1U << 5) | (1U << 13);
    EXTI->FTSR &= ~((1U << 0) | (1U << 5) | (1U << 13));

    /* ---- NVIC ---- */
    NVIC_SetPriority(EXTI0_IRQn,     8);
    NVIC_SetPriority(EXTI9_5_IRQn,   8);
    NVIC_SetPriority(EXTI15_10_IRQn, 8);

    NVIC_EnableIRQ(EXTI0_IRQn);
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/* ---- ISRs ---- */

void EXTI0_IRQHandler(void)
{
    BaseType_t hp = pdFALSE;

    if (EXTI->PR & (1U << 0))
    {
        EXTI->PR = (1U << 0);                 /* Clear pending */
        Sensors_ISR_Handler(0);          /* Signal task */
    }

    portYIELD_FROM_ISR(hp);
}

void EXTI9_5_IRQHandler(void)
{
    BaseType_t hp = pdFALSE;

    if (EXTI->PR & (1U << 5))
    {
        EXTI->PR = (1U << 5);
        Sensors_ISR_Handler(5);
    }

    portYIELD_FROM_ISR(hp);
}

void EXTI15_10_IRQHandler(void)
{
    BaseType_t hp = pdFALSE;

    if (EXTI->PR & (1U << 13))
    {
        EXTI->PR = (1U << 13);
        Sensors_ISR_Handler(13);
    }

    portYIELD_FROM_ISR(hp);
}
