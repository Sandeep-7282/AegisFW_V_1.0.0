/*
 * adc.h
 *
 *  Created on: Dec 14, 2025
 *      Author: sande
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

//#include "stm32f446xx.h"
//
//void adc1_init_temp_sensor(void);
//uint16_t adc1_read_temp_raw(void);


#include "stm32f446xx.h"

typedef struct
{
    uint16_t temp_raw;
    uint16_t vref_raw;
} adc_sample_t;

void adc1_init_temp_vref(void);
void adc1_read_temp_vref(adc_sample_t *sample);

float adc_compute_vref(uint16_t vref_raw);
float adc_compute_temperature(uint16_t temp_raw, float vref);

//newly added
void adc1_init_temp_vref_dma(void);

#endif /* INC_ADC_H_ */
