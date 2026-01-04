/*
 * image_validate.c
 *  Image Vector Validation
 */
#include "image_validate.h"
#include "memory_map.h"

/* Derived boundaries */
#define FLASH_END_ADDR  (FLASH_BASE_ADDR + (512UL * 1024UL))
#define SRAM_END_ADDR   (SRAM_BASE_ADDR  + SRAM_SIZE_BYTES)

img_status_t image_validate_vector(uint32_t app_base)
{
    uint32_t msp   = *(volatile uint32_t *)(app_base + 0x00U);
    uint32_t reset = *(volatile uint32_t *)(app_base + 0x04U);

    /* Zero check */
    if ((msp == 0U) || (reset == 0U))
        return IMG_ERR_ZERO_VECTOR;

    /* MSP must be inside SRAM */
    if ((msp < SRAM_BASE_ADDR) || (msp > SRAM_END_ADDR))
        return IMG_ERR_MSP_RANGE;

    /* MSP must be 8-byte aligned */
    if ((msp & 0x7U) != 0U)
        return IMG_ERR_MSP_ALIGN;

    /* Reset handler must be in flash */
    if ((reset < FLASH_BASE_ADDR) || (reset > FLASH_END_ADDR))
        return IMG_ERR_RESET_RANGE;

    /* Reset handler must be Thumb (LSB = 1) */
    if ((reset & 0x1U) == 0U)
        return IMG_ERR_RESET_THUMB;

    return IMG_OK;
}


