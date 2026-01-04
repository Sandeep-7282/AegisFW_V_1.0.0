/*
 * boot_decision.c
 *
 *  Boot decision logic for AegisFW.
 *  Slot-A is always treated as the latest image and probed first.
 */

#include "boot_decision.h"
#include "boot_metadata.h"
#include "image_validate.h"
#include "image_crc.h"
#include "memory_map.h"
#include "uart.h"
#include <stdbool.h>

uint32_t slot_base(uint8_t slot)
{
    return (slot == SLOT_A) ? APP_SLOT_A_BASE : APP_SLOT_B_BASE;
}

boot_decision_t boot_decide(boot_metadata_t *meta)
{
    uart_puts("Boot decision: probing Slot-A\r\n");

 // 1) Always try Slot-A first (latest image)

    if (image_validate_vector(APP_SLOT_A_BASE) == IMG_OK)
    {
        uint32_t crcA = crc32_compute_flash(APP_SLOT_A_BASE + 8,
                                            APP_SLOT_A_SIZE_BYTES - 8);

        /* First-time enrollment */
        if (meta->crcA == 0)
        {
            uart_puts("Slot-A first boot, enrolling CRC\r\n");
            meta->crcA = crcA;
            meta->active_slot = SLOT_A;
            boot_metadata_store_safe(meta);
            return BOOT_DECISION_BOOT;
        }

        /* CRC match → boot */
        if (meta->crcA == crcA)
        {
            uart_puts("Slot-A CRC OK → booting Slot-A\r\n");
            meta->active_slot = SLOT_A;
            return BOOT_DECISION_BOOT;
        }

        uart_puts("Slot-A CRC mismatch\r\n");
    }
    else
    {
        uart_puts("Slot-A vector invalid\r\n");
    }

  // 2) Slot-A failed → immediate fallback to Slot-B

    uart_puts("Falling back to Slot-B\r\n");

    if (image_validate_vector(APP_SLOT_B_BASE) == IMG_OK)
    {
        uint32_t crcB = crc32_compute_flash(APP_SLOT_B_BASE + 8,
                                            APP_SLOT_B_SIZE_BYTES - 8);

        if (meta->crcB == 0)
        {
            uart_puts("Slot-B first boot, enrolling CRC\r\n");
            meta->crcB = crcB;
            meta->active_slot = SLOT_B;
            boot_metadata_store_safe(meta);
            return BOOT_DECISION_BOOT;
        }

        if (meta->crcB == crcB)
        {
            uart_puts("Slot-B CRC OK → booting Slot-B\r\n");
            meta->active_slot = SLOT_B;
            return BOOT_DECISION_BOOT;
        }

        uart_puts("Slot-B CRC mismatch\r\n");
    }
    else
    {
        uart_puts("Slot-B vector invalid\r\n");
    }

  // 3) Nothing usable → stay in bootloader

    uart_puts("No valid firmware found → staying in bootloader\r\n");
    return BOOT_DECISION_STAY;
}
