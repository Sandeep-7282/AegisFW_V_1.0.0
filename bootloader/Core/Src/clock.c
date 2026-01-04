/*
 * clock.c
 *   Minimal Clock Setup
 */
#include "stm32f446xx.h"
#include "clock.h"

void clock_init_minimal(void)
{
    /* Enable HSI */
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY));

    /*
     * System clock source = HSI
     * AHB  prescaler = 1
     * APB1 prescaler = 1
     * APB2 prescaler = 1
     */
    RCC->CFGR = 0x00000000;

    /*
     * Flash latency:
     * HSI = 16 MHz → 0 wait states is safe
     */
    FLASH->ACR = FLASH_ACR_LATENCY_0WS;
}


