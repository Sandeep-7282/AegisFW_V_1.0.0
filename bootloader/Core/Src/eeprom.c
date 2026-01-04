/*
 * eeprom.c
 *
 *  External EEPROM access via I2C (bootloader use only).
 *
 *  Characteristics:
 *  - Blocking, polling-only
 *  - No interrupts, no DMA
 *  - No timeouts (bootloader simplicity)
 */

#include "stm32f446xx.h"
#include "eeprom.h"

/* 7-bit address (shifted during transfer) */
#define EEPROM_I2C_ADDR  0x50

 //    LOW-LEVEL I2C HELPERS
static void i2c_wait_idle(void)
{
    while (I2C1->SR2 & I2C_SR2_BUSY);
}

static void i2c_start(void)
{
    i2c_wait_idle();
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));
}

static void i2c_send_addr(uint8_t addr, uint8_t read)
{
    I2C1->DR = (addr << 1) | (read ? 1U : 0U);

    while (!(I2C1->SR1 & (I2C_SR1_ADDR | I2C_SR1_AF)));

    if (I2C1->SR1 & I2C_SR1_AF)
    {
        /* NACK received */
        I2C1->SR1 &= ~I2C_SR1_AF;
        return;
    }

    (void)I2C1->SR2; /* Clear ADDR */
}

static void i2c_stop(void)
{
    I2C1->CR1 |= I2C_CR1_STOP;
}

static void i2c_gpio_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    /* PB6 = SCL, PB7 = SDA (AF4, open-drain) */
    GPIOB->MODER   &= ~((3U << (6*2)) | (3U << (7*2)));
    GPIOB->MODER   |=  ((2U << (6*2)) | (2U << (7*2)));

    GPIOB->AFR[0]  |=  (4U << (6*4)) | (4U << (7*4));
    GPIOB->OTYPER  |=  (1U << 6) | (1U << 7);
    GPIOB->OSPEEDR |=  (3U << (6*2)) | (3U << (7*2));

    /* External pull-ups assumed */
    GPIOB->PUPDR   &= ~((3U << (6*2)) | (3U << (7*2)));
}

int eeprom_init(void)
{
    i2c_gpio_init();

    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    /*
     * I2C @ 100 kHz
     * PCLK1 = 16 MHz (HSI)
     */
    I2C1->CR2   = 16;
    I2C1->CCR   = 80;
    I2C1->TRISE = 17;

    I2C1->CR1 |= I2C_CR1_PE | I2C_CR1_ACK;

    return EEPROM_OK;
}

int eeprom_read(uint16_t mem_addr, uint8_t *buf, uint16_t len)
{
    if (!buf || len == 0)
        return EEPROM_ERROR;

    /* Write address pointer */
    i2c_start();
    i2c_send_addr(EEPROM_I2C_ADDR, 0);

    I2C1->DR = (uint8_t)(mem_addr >> 8);
    while (!(I2C1->SR1 & I2C_SR1_TXE));

    I2C1->DR = (uint8_t)(mem_addr & 0xFF);
    while (!(I2C1->SR1 & I2C_SR1_TXE));

    /* Repeated START for read */
    i2c_start();
    i2c_send_addr(EEPROM_I2C_ADDR, 1);

    while (len--)
    {
        if (len == 0)
            I2C1->CR1 &= ~I2C_CR1_ACK; /* Last byte */

        while (!(I2C1->SR1 & I2C_SR1_RXNE));
        *buf++ = I2C1->DR;
    }

    i2c_stop();
    I2C1->CR1 |= I2C_CR1_ACK;

    return EEPROM_OK;
}

static void eeprom_wait_ready(void)
{
    /* Poll ACK until internal write cycle completes */
    while (1)
    {
        i2c_start();
        I2C1->DR = (EEPROM_I2C_ADDR << 1);

        while (!(I2C1->SR1 & (I2C_SR1_ADDR | I2C_SR1_AF)));

        if (I2C1->SR1 & I2C_SR1_ADDR)
        {
            (void)I2C1->SR2;
            i2c_stop();
            break;
        }

        I2C1->SR1 &= ~I2C_SR1_AF;
    }
}

int eeprom_write(uint16_t mem_addr, const uint8_t *buf, uint16_t len)
{
    if (!buf || len == 0)
        return EEPROM_ERROR;

    while (len > 0)
    {
        uint16_t page_offset = mem_addr % EEPROM_PAGE_SIZE;
        uint16_t chunk       = EEPROM_PAGE_SIZE - page_offset;

        if (chunk > len)
            chunk = len;

        i2c_start();
        i2c_send_addr(EEPROM_I2C_ADDR, 0);

        I2C1->DR = (uint8_t)(mem_addr >> 8);
        while (!(I2C1->SR1 & I2C_SR1_TXE));

        I2C1->DR = (uint8_t)(mem_addr & 0xFF);
        while (!(I2C1->SR1 & I2C_SR1_TXE));

        for (uint16_t i = 0; i < chunk; i++)
        {
            I2C1->DR = buf[i];
            while (!(I2C1->SR1 & I2C_SR1_TXE));
        }

        i2c_stop();
        eeprom_wait_ready();

        mem_addr += chunk;
        buf      += chunk;
        len      -= chunk;
    }

    return EEPROM_OK;
}
