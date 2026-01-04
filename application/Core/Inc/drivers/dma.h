/*
 * dma.h
 *
 *  Created on: Dec 14, 2025
 *      Author: sande
 */

#ifndef INC_DMA_H_
#define INC_DMA_H_


#include "stm32f446xx.h"

#define ADC_DMA_BUF_LEN   64   /* Must be even (TEMP,VREF pairs) */

extern volatile uint16_t adc_dma_buffer[ADC_DMA_BUF_LEN];

void dma2_adc1_init(void);

#endif /* INC_DMA_H_ */
