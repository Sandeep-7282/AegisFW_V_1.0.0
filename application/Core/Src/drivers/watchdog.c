/*
 * watchdog.c
 *
 *  Independent Watchdog (IWDG) driver.
 *
 *  Responsibility:
 *  - Configure IWDG
 *  - Feed IWDG
 *
 */

#include "watchdog.h"
#include "stm32f4xx.h"

void Watchdog_Init(void)
{
    /*
     * LSI ~32 kHz
     * Prescaler = 64
     * Reload = 2000 → ~4 seconds timeout
     */
    IWDG->KR = 0x5555;      /* Enable write access */
    IWDG->PR = 0x03;        /* Prescaler */
    IWDG->RLR = 2000;
    IWDG->KR = 0xAAAA;      /* Reload counter */
    IWDG->KR = 0xCCCC;      /* Start watchdog */
}

void Watchdog_Feed(void)
{
    IWDG->KR = 0xAAAA;
}
