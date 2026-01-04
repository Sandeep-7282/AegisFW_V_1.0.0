/*
 * flash_if.c
 * flash operations on SLOT A
 */
#include "flash_if.h"
#include "stm32f446xx.h"
#include "string.h"

static void flash_unlock(void)
{
    if (FLASH->CR & FLASH_CR_LOCK)
    {
        FLASH->KEYR = 0x45670123;
        FLASH->KEYR = 0xCDEF89AB;
    }
}

static void flash_lock(void)
{
    FLASH->CR |= FLASH_CR_LOCK;
}

bool flash_erase_slot_a(void)
{
    flash_unlock();

    const uint8_t sectors[] = {3, 4, 5};

    for (uint32_t i = 0; i < sizeof(sectors); i++)
    {
        while (FLASH->SR & FLASH_SR_BSY);

        FLASH->CR = FLASH_CR_SER |
                    (sectors[i] << FLASH_CR_SNB_Pos);

        FLASH->CR |= FLASH_CR_STRT;

        while (FLASH->SR & FLASH_SR_BSY);

        if (FLASH->SR & (FLASH_SR_OPERR |
                         FLASH_SR_WRPERR |
                         FLASH_SR_PGAERR |
                         FLASH_SR_PGPERR |
                         FLASH_SR_PGSERR))
        {
            flash_lock();
            return false;
        }
    }

    flash_lock();
    return true;
}


bool flash_program_buffer(uint32_t addr,
                          const uint8_t *data,
                          uint32_t len)
{
    flash_unlock();

    for (uint32_t i = 0; i < len; i += 4)
    {
        uint32_t word = 0xFFFFFFFF;

        uint32_t remaining = len - i;
        if (remaining >= 4)
        {
            word = *(uint32_t *)(data + i);
        }
        else
        {
            memcpy(&word, data + i, remaining);
        }

        while (FLASH->SR & FLASH_SR_BSY);

        FLASH->CR |= FLASH_CR_PG;
        *(volatile uint32_t *)(addr + i) = word;

        while (FLASH->SR & FLASH_SR_BSY);
        FLASH->CR &= ~FLASH_CR_PG;

        if (*(volatile uint32_t *)(addr + i) != word)
        {
            flash_lock();
            return false;
        }
    }

    flash_lock();
    return true;
}



