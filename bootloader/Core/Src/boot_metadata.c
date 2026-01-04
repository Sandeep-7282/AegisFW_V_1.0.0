/*
 * boot_metadata.c
 *
 *  Persistent boot metadata handling.
 *
 *  Provides:
 *  - Dual-copy EEPROM storage
 *  - Corruption tolerance
 *  - Slot selection assistance
 *
 */

#include "boot_metadata.h"
#include "eeprom.h"
#include "uart.h"
#include "memory_map.h"
#include "image_validate.h"
#include <stdbool.h>
#include <stdio.h>

uint32_t metadata_checksum(const boot_metadata_t *m)
{
    const uint8_t *p = (const uint8_t *)m;
    uint32_t sum = 0;

    for (uint32_t i = 0; i < sizeof(boot_metadata_t) - sizeof(uint32_t); i++)
        sum += p[i];

    return sum;
}

bool slot_is_valid(uint32_t base)
{
    return (image_validate_vector(base) == IMG_OK);
}

void boot_metadata_default(boot_metadata_t *m)
{
    m->magic       = BOOT_METADATA_MAGIC;
    m->version     = BOOT_METADATA_VERSION;
    m->length      = sizeof(boot_metadata_t);

    m->active_slot = SLOT_A;
    m->crcA        = 0;
    m->crcB        = 0;
    m->flags       = META_FLAG_VALID;

    m->checksum    = metadata_checksum(m);
}

static int metadata_try_load(uint16_t addr, boot_metadata_t *m)
{
    if (eeprom_read(addr, (uint8_t *)m, sizeof(*m)) != EEPROM_OK)
        return -1;

    if (m->magic    != BOOT_METADATA_MAGIC)   return -1;
    if (m->version  != BOOT_METADATA_VERSION) return -1;
    if (m->length   != sizeof(*m))            return -1;
    if (m->checksum != metadata_checksum(m))  return -1;

    return 0;
}

int boot_metadata_load_or_init(boot_metadata_t *meta)
{
    boot_metadata_t m0, m1;
    int v0 = metadata_try_load(METADATA_SLOT0_ADDR, &m0);
    int v1 = metadata_try_load(METADATA_SLOT1_ADDR, &m1);

    bool A_ok = slot_is_valid(APP_SLOT_A_BASE);
    bool B_ok = slot_is_valid(APP_SLOT_B_BASE);

    if (v0 != 0 && v1 != 0)
    {
        uart_puts("Metadata: none found, creating defaults\r\n");
        boot_metadata_default(meta);
        boot_metadata_store_safe(meta);
        return 0;
    }

    if (v0 == 0 && v1 != 0)
    {
        *meta = m0;
        return 0;
    }

    if (v1 == 0 && v0 != 0)
    {
        *meta = m1;
        return 0;
    }

    /* Both valid — resolve conflicts */
    uart_puts("Metadata: resolving dual copies\r\n");

    if (A_ok && !B_ok)
    {
        *meta = m0;
        meta->active_slot = SLOT_A;
    }
    else if (!A_ok && B_ok)
    {
        *meta = m1;
        meta->active_slot = SLOT_B;
    }
    else
    {
        *meta = (m0.active_slot == SLOT_A) ? m0 : m1;
    }

    boot_metadata_store_safe(meta);
    return 0;
}

int boot_metadata_load(boot_metadata_t *meta)
{
    return boot_metadata_load_or_init(meta);
}

int boot_metadata_store_safe(const boot_metadata_t *meta)
{
    boot_metadata_t tmp;
    uint16_t target;

    target = (metadata_try_load(METADATA_SLOT0_ADDR, &tmp) == 0) ?
              METADATA_SLOT1_ADDR : METADATA_SLOT0_ADDR;

    boot_metadata_t m = *meta;
    m.checksum = metadata_checksum(&m);

    return eeprom_write(target, (uint8_t *)&m, sizeof(m));
}

void boot_metadata_dump(const boot_metadata_t *m)
{
    uart_puts("---- BOOT METADATA ----\r\n");

    printf("Active Slot : %c\r\n", (m->active_slot == SLOT_A) ? 'A' : 'B');
    printf("CRC Slot A  : 0x%08lX\r\n", m->crcA);
    printf("CRC Slot B  : 0x%08lX\r\n", m->crcB);
    printf("Checksum   : 0x%08lX\r\n", m->checksum);

    uart_puts("-----------------------\r\n");
}
