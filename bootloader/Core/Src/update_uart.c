/*
 * update_uart.c
 *
 *  Update Through UART
 */

#include "update_uart.h"
#include "uart.h"
#include "update_protocol.h"
#include "flash_if.h"
#include "memory_map.h"
#include "image_crc.h"
#include "boot_metadata.h"
#include "stm32f446xx.h"
#include "stdint.h"
#include "stdbool.h"

// Forward declarations

static bool mark_slot_a_pending(uint32_t crc, uint32_t version);

// UART helpers (bootloader)

bool bl_uart_recv(uint8_t *buf, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++)
    {
        buf[i] = (uint8_t)uart_getc();   /* blocking */
    }
    return true;
}

void bl_uart_send_str(const char *s)
{
    uart_puts(s);
}

// Firmware update logic

#define CHUNK_SIZE 256

bool bl_receive_and_program_image(void)
{
    fw_update_header_t hdr;
    uint8_t  buffer[CHUNK_SIZE];
    uint32_t bytes_received = 0;
    uint32_t write_addr     = APP_SLOT_A_BASE;

    bl_uart_send_str("BL: Waiting for update header...\r\n");

    /* Receive header */
    if (!bl_uart_recv((uint8_t *)&hdr, sizeof(hdr)))
        return false;

    /* Validate header */
    if (hdr.magic != FW_UPDATE_MAGIC)
    {
        bl_uart_send_str("BL: Invalid update magic\r\n");
        return false;
    }

    if (hdr.image_size == 0 || hdr.image_size > APP_SLOT_A_SIZE_BYTES)
    {
        bl_uart_send_str("BL: Invalid image size\r\n");
        return false;
    }

    /* Erase Slot A */
    bl_uart_send_str("BL: Erasing Slot A...\r\n");

    if (!flash_erase_slot_a())
    {
        bl_uart_send_str("BL: Flash erase failed\r\n");
        return false;
    }

    /* Receive and program image */
    bl_uart_send_str("BL: Receiving image...\r\n");

    while (bytes_received < hdr.image_size)
    {
        uint32_t chunk = CHUNK_SIZE;
        if ((hdr.image_size - bytes_received) < CHUNK_SIZE)
            chunk = hdr.image_size - bytes_received;

        if (!bl_uart_recv(buffer, chunk))
        {
            bl_uart_send_str("BL: UART receive failed\r\n");
            return false;
        }

        if (!flash_program_buffer(write_addr, buffer, chunk))
        {
            bl_uart_send_str("BL: Flash program failed\r\n");
            return false;
        }

        write_addr     += chunk;
        bytes_received += chunk;
    }

    bl_uart_send_str("BL: Image received\r\n");
    bl_uart_send_str("BL: Verifying CRC...\r\n");

    /* CRC verification */
    uint32_t computed_crc =
    		crc32_compute((uint8_t *)(APP_SLOT_A_BASE + 8),
    		              hdr.image_size - 8);

    if (computed_crc != hdr.image_crc)
    {
        bl_uart_send_str("BL: CRC mismatch\r\n");

        /* Explicitly clear pending state if any */
        boot_metadata_t meta;
        if (boot_metadata_load(&meta) == 0)
        {
            meta.flags &= ~META_FLAG_PENDING;
            boot_metadata_store_safe(&meta);
        }

        return false;
    }

    bl_uart_send_str("BL: CRC OK\r\n");
    bl_uart_send_str("BL: Marking Slot-A as pending...\r\n");

    /* Mark Slot A as TRIAL (pending) */
    if (!mark_slot_a_pending(hdr.image_crc, hdr.version))
    {
        bl_uart_send_str("BL: Metadata update failed\r\n");
        return false;
    }

    bl_uart_send_str("BL: Update complete, rebooting...\r\n");

    /* Allow UART to flush */
    for (volatile uint32_t i = 0; i < 100000; i++);

    NVIC_SystemReset();

    /* Should never reach here */
    return true;
}

// Metadata update helper

static bool mark_slot_a_pending(uint32_t crc, uint32_t version)
{
    boot_metadata_t meta;

    if (boot_metadata_load(&meta) != 0)
        return false;

    meta.active_slot   = SLOT_A;
    meta.boot_attempts = 0;

    meta.crcA    = crc;
    meta.version = version;

    meta.flags &= ~META_FLAG_ROLLBACK;
    meta.flags |=  META_FLAG_PENDING;

    return (boot_metadata_store_safe(&meta) == 0);
}
