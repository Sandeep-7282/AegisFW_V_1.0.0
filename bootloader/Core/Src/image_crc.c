/*
 * image_crc.c
 *
 *  CRC32 computation for image validation.
 *
 */

#include "image_crc.h"

#define CRC32_POLY  0x04C11DB7UL
#define CRC32_INIT  0xFFFFFFFFUL

static uint32_t crc32_update(uint32_t crc, uint8_t data)
{
    crc ^= ((uint32_t)data << 24);

    for (uint8_t i = 0; i < 8; i++)
    {
        crc = (crc & 0x80000000UL) ? (crc << 1) ^ CRC32_POLY
                                  : (crc << 1);
    }
    return crc;
}

uint32_t crc32_compute_flash(uint32_t start_addr, uint32_t length_bytes)
{
    uint32_t crc = CRC32_INIT;
    const uint8_t *p = (const uint8_t *)start_addr;

    for (uint32_t i = 0; i < length_bytes; i++)
        crc = crc32_update(crc, p[i]);

    return ~crc;
}

uint32_t crc32_compute(const uint8_t *data, uint32_t len)
{
    uint32_t crc = CRC32_INIT;

    for (uint32_t i = 0; i < len; i++)
        crc = crc32_update(crc, data[i]);

    return ~crc;
}
