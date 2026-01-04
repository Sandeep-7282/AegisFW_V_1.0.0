/*
 * flash_ops.c
 *
 *  Flash sector erase (bare-metal).
 *
 */

#include "flash_ops.h"
#include "stm32f446xx.h"

static uint32_t address_to_sector(uint32_t addr)
{
    if (addr < 0x08004000) return 0;
    if (addr < 0x08008000) return 1;
    if (addr < 0x0800C000) return 2;
    if (addr < 0x08010000) return 3;
    if (addr < 0x08020000) return 4;
    if (addr < 0x08040000) return 5;
    if (addr < 0x08060000) return 6;
    return 7;
}

void flash_erase_range(uint32_t address, uint32_t size)
{
    uint32_t start = address_to_sector(address);
    uint32_t end   = address_to_sector(address + size - 1);

    if (FLASH->CR & FLASH_CR_LOCK)
    {
        FLASH->KEYR = 0x45670123;
        FLASH->KEYR = 0xCDEF89AB;
    }

    for (uint32_t s = start; s <= end; s++)
    {
        while (FLASH->SR & FLASH_SR_BSY);

        FLASH->CR = FLASH_CR_SER | (s << FLASH_CR_SNB_Pos);
        FLASH->CR |= FLASH_CR_STRT;

        while (FLASH->SR & FLASH_SR_BSY);
        FLASH->CR &= ~FLASH_CR_SER;
    }

    FLASH->CR |= FLASH_CR_LOCK;
}
