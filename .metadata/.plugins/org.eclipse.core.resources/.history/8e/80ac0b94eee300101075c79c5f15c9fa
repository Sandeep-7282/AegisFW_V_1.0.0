/*
 * dwt.c
 *
 *  Created on: Dec 27, 2025
 *      Author: sande
 */
#include "dwt.h"
#include "stm32f4xx.h"

void dwt_init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

uint32_t dwt_get_cycle(void)
{
    return DWT->CYCCNT;
}


