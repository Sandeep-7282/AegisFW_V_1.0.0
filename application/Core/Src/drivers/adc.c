/*
 * adc.c
 *
 *  ADC driver for internal temperature sensor and VREFINT.
 *
 *  Production-grade implementation for AegisFW.
 */

#include "adc.h"
#include "stm32f446xx.h"

/* Factory calibration values (STM32F446 reference manual) */
#define VREFINT_CAL_ADDR   ((uint16_t *)0x1FFF7A2A)
#define TEMP30_CAL_ADDR    ((uint16_t *)0x1FFF7A2C)
#define TEMP110_CAL_ADDR   ((uint16_t *)0x1FFF7A2E)

static void adc_delay(void)
{
    for (volatile uint32_t i = 0; i < 10000; i++);
}

/*
 * One-shot (polling) ADC init
 */
void adc1_init_temp_vref(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    /* ADC common configuration */
    ADC->CCR &= ~ADC_CCR_ADCPRE_Msk;
    ADC->CCR |=  (0x1U << ADC_CCR_ADCPRE_Pos);   /* PCLK2 / 4 */
    ADC->CCR |=  ADC_CCR_TSVREFE;

    /* Reset ADC */
    ADC1->CR1 = 0;
    ADC1->CR2 = 0;

    ADC1->CR1 |= ADC_CR1_SCAN;
    ADC1->CR2 |= ADC_CR2_EOCS;

    /* Sample times */
    ADC1->SMPR1 &= ~((7U << 18) | (7U << 21));
    ADC1->SMPR1 |=  (4U << 18) | (4U << 21);    /* 84 cycles */

    /* Sequence length = 2 */
    ADC1->SQR1 &= ~(0xFU << 20);
    ADC1->SQR1 |=  (1U << 20);

    /* TEMP -> VREFINT */
    ADC1->SQR3 = (16U << 0) | (17U << 5);

    /* Enable ADC (dummy enable sequence) */
    ADC1->CR2 |= ADC_CR2_ADON;
    adc_delay();
    ADC1->CR2 |= ADC_CR2_ADON;
}

/*
 * Polling read
 */
void adc1_read_temp_vref(adc_sample_t *sample)
{
    ADC1->CR2 |= ADC_CR2_SWSTART;

    while (!(ADC1->SR & ADC_SR_EOC));
    sample->temp_raw = ADC1->DR;

    while (!(ADC1->SR & ADC_SR_EOC));
    sample->vref_raw = ADC1->DR;
}

/*
 * Continuous ADC + DMA mode init
 */
void adc1_init_temp_vref_dma(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    ADC->CCR &= ~ADC_CCR_ADCPRE_Msk;
    ADC->CCR |=  (0x1U << ADC_CCR_ADCPRE_Pos);
    ADC->CCR |=  ADC_CCR_TSVREFE;

    ADC1->CR1 = 0;
    ADC1->CR2 = 0;

    ADC1->CR1 |= ADC_CR1_SCAN;

    ADC1->CR2 |= ADC_CR2_DMA;
    ADC1->CR2 |= ADC_CR2_DDS;
    ADC1->CR2 |= ADC_CR2_CONT;

    /* Ensure EOCS is cleared in DMA mode */
    ADC1->CR2 &= ~ADC_CR2_EOCS;

    ADC1->SMPR1 &= ~((7U << 18) | (7U << 21));
    ADC1->SMPR1 |=  (4U << 18) | (4U << 21);

    ADC1->SQR1 &= ~(0xFU << 20);
    ADC1->SQR1 |=  (1U << 20);

    ADC1->SQR3 = (16U << 0) | (17U << 5);

    /* Enable ADC */
    ADC1->CR2 |= ADC_CR2_ADON;
    adc_delay();
    ADC1->CR2 |= ADC_CR2_ADON;
}

/*
 * Conversion helpers (fault-safe)
 */
float adc_compute_vref(uint16_t vref_raw)
{
    if (vref_raw == 0U)
        return 0.0f;

    return 3.3f * ((float)(*VREFINT_CAL_ADDR) / (float)vref_raw);
}

float adc_compute_temperature(uint16_t temp_raw, float vref)
{
    if (vref <= 0.0f)
        return 0.0f;

    float vsense  = (temp_raw * vref) / 4095.0f;
    float temp30  = (*TEMP30_CAL_ADDR  * vref) / 4095.0f;
    float temp110 = (*TEMP110_CAL_ADDR * vref) / 4095.0f;

    return ((vsense - temp30) * (110.0f - 30.0f) /
           (temp110 - temp30)) + 30.0f;
}
