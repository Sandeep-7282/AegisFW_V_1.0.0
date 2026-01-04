/*
 * i2c.c
 *
 *  Bare-metal I2C2 master driver (STM32F446).
 *  Hardened for AegisFW.
 */

#include "i2c.h"
#include "stm32f446xx.h"

#define I2C_TIMEOUT  100000UL

static int i2c_wait_flag(volatile uint32_t *reg,
                         uint32_t flag,
                         uint8_t set)
{
    uint32_t timeout = I2C_TIMEOUT;
    while (((*reg & flag) != 0U) != set)
    {
        if (--timeout == 0U)
            return -1;
    }
    return 0;
}

void i2c2_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

    /* PB10=SCL, PB11=SDA (AF4, open-drain) */
    GPIOB->MODER   &= ~((3U << 20) | (3U << 22));
    GPIOB->MODER   |=  ((2U << 20) | (2U << 22));
    GPIOB->AFR[1]  &= ~((0xFU << 8) | (0xFU << 12));
    GPIOB->AFR[1]  |=  (4U << 8) | (4U << 12);
    GPIOB->OTYPER  |=  (1U << 10) | (1U << 11);
    GPIOB->OSPEEDR |=  (3U << 20) | (3U << 22);
    GPIOB->PUPDR   &= ~((3U << 20) | (3U << 22));
    GPIOB->PUPDR   |=  (1U << 20) | (1U << 22);

    /* Reset I2C */
    I2C2->CR1 |= I2C_CR1_SWRST;
    I2C2->CR1 &= ~I2C_CR1_SWRST;

    /* Configure timing for 16 MHz PCLK1, 100 kHz */
    I2C2->CR2   = 16;
    I2C2->CCR   = 80;
    I2C2->TRISE = 17;

    /* Enable ACK */
    I2C2->CR1 |= I2C_CR1_ACK;

    /* Enable I2C */
    I2C2->CR1 |= I2C_CR1_PE;
}

/*
 * Blocking master transmit with timeout
 */
void i2c2_write(uint8_t addr, uint8_t *data, uint16_t len)
{
    if (!data || len == 0)
        return ;

    /* Wait until bus is free */
    if (i2c_wait_flag(&I2C2->SR2, I2C_SR2_BUSY, 0) < 0)
        return ;

    /* START condition */
    I2C2->CR1 |= I2C_CR1_START;
    if (i2c_wait_flag(&I2C2->SR1, I2C_SR1_SB, 1) < 0)
        goto error;

    (void)I2C2->SR1;
    I2C2->DR = addr << 1;  /* write */

    if (i2c_wait_flag(&I2C2->SR1, I2C_SR1_ADDR, 1) < 0)
        goto error;

    (void)I2C2->SR1;
    (void)I2C2->SR2;

    for (uint16_t i = 0; i < len; i++)
    {
        if (i2c_wait_flag(&I2C2->SR1, I2C_SR1_TXE, 1) < 0)
            goto error;

        I2C2->DR = data[i];
    }

    if (i2c_wait_flag(&I2C2->SR1, I2C_SR1_BTF, 1) < 0)
        goto error;

    I2C2->CR1 |= I2C_CR1_STOP;
    return ;

error:
    I2C2->CR1 |= I2C_CR1_STOP;
    return ;
}
